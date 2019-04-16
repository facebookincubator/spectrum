// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/IDecompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/image/metadata/Entries.h>
#include <spectrum/io/IImageSource.h>

#include <dav1d/dav1d.h>
#include <folly/Optional.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {

/**
 * IvfAv1Decompressor reads IVF files with an AV1 payload using dav1d.
 */
class IvfAv1Decompressor final : public codecs::IDecompressor {
 public:
  explicit IvfAv1Decompressor(io::IImageSource& source);
  IvfAv1Decompressor(IvfAv1Decompressor&&) = default;
  ~IvfAv1Decompressor() override;

 private:
  Dav1dSettings _dav1dSettings;
  Dav1dContext* _dav1dContext = nullptr;
  io::IImageSource& _source;

  folly::Optional<image::Specification> _imageSpecification;
  size_t _payloadLength;
  void _ensureHeaderIsRead();

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
