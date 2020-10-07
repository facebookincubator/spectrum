// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/IDecompressor.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSource.h>

#include <folly/Optional.h>

#include <memory>

extern "C" {
struct avifDecoder;
}

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {

/**
 * AvifDecompressor reads AVIF files using libavif.
 */
class AvifDecompressor final : public codecs::IDecompressor {
 public:
  explicit AvifDecompressor(io::IImageSource& source);
  AvifDecompressor(AvifDecompressor&&) = default;

  virtual ~AvifDecompressor();

 private:
  io::IImageSource& _source;

  avifDecoder* _decoder = nullptr;
  void _parseContainer();

  folly::Optional<image::Specification> _imageSpecification;
  void _computeSpecifications();

  bool _entireImageHasBeenRead = false;
  std::vector<std::unique_ptr<image::Scanline>> _entireImage;
  std::uint32_t _currentOutputScanline = 0;
  void _ensureEntireImageIsRead();

 public:
  image::Specification sourceImageSpecification() override;
  image::Specification outputImageSpecification() override;

  std::unique_ptr<image::Scanline> readScanline() override;
};

} // namespace avif
} // namespace plugins
} // namespace spectrum
} // namespace facebook
