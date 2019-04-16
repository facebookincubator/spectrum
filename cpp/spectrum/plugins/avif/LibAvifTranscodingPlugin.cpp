// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibAvifTranscodingPlugin.h"

#include <ivf/ivfheader.h>
#include <spectrum/Rule.h>
#include <spectrum/Spectrum.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/plugins/avif/IvfAv1Decompressor.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {

namespace {

constexpr auto IvfPrefix = folly::StringPiece{"DKIF"};
constexpr auto Av1FourCC = folly::StringPiece{"AV01"};

folly::Optional<image::EncodedFormat> ivfEncodedImageFormatDetectorHandler(
    io::IImageSource& source) {
  constexpr auto len = sizeof(fb::ivf::IvfFileHeader);

  std::vector<char> bytes(len);
  if (source.read(bytes.data(), len) != len) {
    return folly::none; // header too short
  }

  if (strncmp(IvfPrefix.begin(), bytes.data(), IvfPrefix.size()) != 0) {
    return folly::none; // not a IVF header
  }

  const auto data = reinterpret_cast<const std::uint8_t*>(bytes.data());
  auto range = folly::Range<const std::uint8_t*>(data, data + len);

  const auto ivfHeader = fb::ivf::parseIvfFileHeader(range);
  if (strncmp(
          Av1FourCC.begin(),
          reinterpret_cast<const char*>(ivfHeader.fourcc),
          Av1FourCC.size()) == 0) {
    return formats::IvfAv1; // fourcc indicating AV1
  }

  return folly::none;
}

inline codecs::DecompressorProvider::Factory makeIvfAv1DecompressorFactory() {
  return [](io::IImageSource& source,
            const folly::Optional<image::Ratio>& /* unused */,
            const Configuration& /* unused */) {
    return std::make_unique<IvfAv1Decompressor>(source);
  };
}

codecs::DecompressorProvider makeIvfAv1DecompressorProvider() {
  return {
      .format = formats::IvfAv1,
      .supportedSamplingRatios = {},
      .decompressorFactory = makeIvfAv1DecompressorFactory(),
  };
}

} // namespace

Plugin makeTranscodingPlugin() {
  auto plugin = Plugin{};
  plugin.encodedImageFormatDetectorHandlers.push_back(
      &ivfEncodedImageFormatDetectorHandler);
  plugin.decompressorProviders.push_back(makeIvfAv1DecompressorProvider());
  return plugin;
}

} // namespace avif
} // namespace plugins
} // namespace spectrum
} // namespace facebook
