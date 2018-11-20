// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "EncodedImageFormatDetectorHandlers.h"

#include <spectrum/SpectrumException.h>
#include <spectrum/codecs/isobmff/IsoBmffParser.h>

#include <algorithm>
#include <array>
#include <exception>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace error {
const folly::StringPiece EncodedImageFormatDetectorHandlerEmptyHeader{
    "format_detector_empty_header"};
}

namespace headers {
constexpr auto Jpeg = folly::StringPiece{"\xFF\xD8\xFF"};
constexpr auto Png = folly::StringPiece{"\x89PNG\x0D\x0A\x1A\x0A"};
constexpr auto Gif87a = folly::StringPiece{"GIF87a"};
constexpr auto Gif89a = folly::StringPiece{"GIF89a"};
constexpr auto Riff = folly::StringPiece{"RIFF"};
constexpr auto RiffWebpVp8 = folly::StringPiece{"WEBPVP8 "};
constexpr auto RiffWebpVp8l = folly::StringPiece{"WEBPVP8L"};
constexpr auto RiffWebpVp8x = folly::StringPiece{"WEBPVP8X"};
} // namespace headers

namespace {

EncodedImageFormatDetectorHandler makeRiffImageFormatDetectorHandler(
    const folly::StringPiece& subHeader,
    const image::EncodedFormat& imageFormat) {
  // Riff format contains RIFF encoded in the first 4 bytes. Then 4 bytes are
  // used for the size. The following bytes represent the subHeader type.
  constexpr auto riffHeaderSubHeaderPadding = 4;
  const auto subHeaderOffsetFromStart =
      headers::Riff.size() + riffHeaderSubHeaderPadding;
  const auto totalSize = subHeaderOffsetFromStart + subHeader.size();

  return
      [=](io::IImageSource& source) -> folly::Optional<image::EncodedFormat> {
        std::vector<char> buffer(totalSize);
        const auto bytesRead = source.read(buffer.data(), totalSize);
        // Below we're doing two strncmp, one for the RIFF header and one at
        // offset subHeaderOffsetFromStart to compare the sub-header.
        if (bytesRead == totalSize &&
            std::strncmp(
                buffer.data(), headers::Riff.begin(), headers::Riff.size()) ==
                0 &&
            std::strncmp(
                buffer.data() + subHeaderOffsetFromStart,
                subHeader.begin(),
                subHeader.size()) == 0) {
          return imageFormat;
        } else {
          return folly::none;
        }
      };
}

constexpr std::array<folly::StringPiece, 6> heifBrands = {
    {folly::StringPiece{"mif1"},
     folly::StringPiece{"msf1"},
     folly::StringPiece{"heic"},
     folly::StringPiece{"heix"},
     folly::StringPiece{"hevc"},
     folly::StringPiece{"hevx"}}};

bool isHeifBrand(const isobmff::Brand& brand) {
  const char* brandChars = reinterpret_cast<const char*>(brand.data());
  for (const auto& brand : heifBrands) {
    if (strncmp(brand.begin(), brandChars, brand.size()) == 0) {
      return true;
    }
  }
  return false;
}

folly::Optional<image::EncodedFormat> heifEncodedImageFormatDetectorHandler(
    io::IImageSource& source) {
  constexpr uint64_t maxBoxSize = 64;

  auto parser = isobmff::Parser(source, maxBoxSize);

  isobmff::FtypBox ftypBox;
  try {
    ftypBox = parser.parseFtypBox();
  } catch (SpectrumException& ignore) {
    // any error in the parser is indicating that we are not reading a HEIF file
    return folly::none;
  }

  if (isHeifBrand(ftypBox.majorBrand)) {
    return image::formats::Heif;
  }

  for (const auto& brand : ftypBox.compatibleBrands) {
    if (isHeifBrand(brand)) {
      return image::formats::Heif;
    }
  }

  return folly::none;
}

} // namespace

EncodedImageFormatDetectorHandler makeSimpleImageFormatDetectorHandler(
    const folly::StringPiece& header,
    const image::EncodedFormat& imageFormat) {
  SPECTRUM_ERROR_IF(
      header.empty(), error::EncodedImageFormatDetectorHandlerEmptyHeader);

  return
      [=](io::IImageSource& source) -> folly::Optional<image::EncodedFormat> {
        std::vector<char> buffer(header.size());
        const auto bytesRead = source.read(buffer.data(), header.size());
        if (bytesRead == header.size() &&
            std::strncmp(buffer.data(), header.data(), bytesRead) == 0) {
          return imageFormat;
        } else {
          return folly::none;
        }
      };
}

EncodedImageFormatDetectorHandler makeJpegImageFormatDetectorHandler() {
  return makeSimpleImageFormatDetectorHandler(
      headers::Jpeg, image::formats::Jpeg);
}

EncodedImageFormatDetectorHandler makePngImageFormatDetectorHandler() {
  return makeSimpleImageFormatDetectorHandler(
      headers::Png, image::formats::Png);
}

EncodedImageFormatDetectorHandler makeGif87ImageFormatDetectorHandler() {
  return makeSimpleImageFormatDetectorHandler(
      headers::Gif87a, image::formats::Gif);
}

EncodedImageFormatDetectorHandler makeGif89ImageFormatDetectorHandler() {
  return makeSimpleImageFormatDetectorHandler(
      headers::Gif89a, image::formats::Gif);
}

EncodedImageFormatDetectorHandler makeWebpVp8ImageFormatDetectorHandler() {
  return makeRiffImageFormatDetectorHandler(
      headers::RiffWebpVp8, image::formats::Webp);
}

EncodedImageFormatDetectorHandler makeWebpVp8lImageFormatDetectorHandler() {
  return makeRiffImageFormatDetectorHandler(
      headers::RiffWebpVp8l, image::formats::Webp);
}

EncodedImageFormatDetectorHandler makeWebpVp8xImageFormatDetectorHandler() {
  return makeRiffImageFormatDetectorHandler(
      headers::RiffWebpVp8x, image::formats::Webp);
}

EncodedImageFormatDetectorHandler makeHeifImageFormatDetectorHandler() {
  return &heifEncodedImageFormatDetectorHandler;
}

std::vector<EncodedImageFormatDetectorHandler>
makeAllImageFormatDetectorHandlers() {
  return {
      makeJpegImageFormatDetectorHandler(),
      makeGif87ImageFormatDetectorHandler(),
      makeGif89ImageFormatDetectorHandler(),
      makePngImageFormatDetectorHandler(),
      makeWebpVp8ImageFormatDetectorHandler(),
      makeWebpVp8lImageFormatDetectorHandler(),
      makeWebpVp8xImageFormatDetectorHandler(),
      makeHeifImageFormatDetectorHandler(),
  };
}

} // namespace codecs
} // namespace spectrum
} // namespace facebook
