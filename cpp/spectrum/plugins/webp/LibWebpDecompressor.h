// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/IDecompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSource.h>

#include <folly/Optional.h>
#include <webp/decode.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace webp {

/**
 * LibWebpDecompressor is a wrapper around libwebp. It manages the underlying
 * resource, handles the conversion to Spectrum types and helps to prevent
 * allocation and logic errors.
 */
class LibWebpDecompressor final : public codecs::IDecompressor {
 public:
  explicit LibWebpDecompressor(
      io::IImageSource& source,
      const folly::Optional<image::Ratio>& samplingRatio = folly::none,
      const folly::Optional<image::pixel::Specification>&
          overridePixelSpecification = folly::none);

  LibWebpDecompressor(LibWebpDecompressor&&) = default;

  ~LibWebpDecompressor() override;

 private:
  io::IImageSource& _source;
  folly::Optional<image::pixel::Specification> _overridePixelSpecification;

  // libwebp's struct to maintain state
  WebPIDecoder* _webpDecoder = nullptr;

  // contains width, height, has_alpha and similar header fields
  WebPBitstreamFeatures _webpFeatures;

  // buffer with the raw pixel data after decoding
  std::vector<std::uint8_t> _rawImage;

  // we have to cache read bytes from the header parsing to re-read it when
  // decoding the entire image; should not exceed a few bytes and gets cleared
  // once the entire image is read
  std::vector<char> _webpPayload;
  bool _isHeaderRead = false;

  std::size_t _outputScanline = 0;
  bool _isImageRead = false;

  void _ensureHeaderIsRead();
  void _ensureImageIsRead();

  folly::Optional<image::Specification> _sourceImageSpecification;

 public:
  //
  // Decompressor
  //

  image::Specification sourceImageSpecification() override;
  image::Specification outputImageSpecification() override;

  std::unique_ptr<image::Scanline> readScanline() override;
};

} // namespace webp
} // namespace plugins
} // namespace spectrum
} // namespace facebook
