// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/DecompressorProvider.h>
#include <spectrum/codecs/IDecompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/image/metadata/Entries.h>
#include <spectrum/io/IBitmapImageSource.h>

#include <array>
#include <memory>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace bitmap {

/**
 * The BitmapDecompressor will read the scanlines directly from the input
 * source. It relies on the provided image information for the correct bitmap
 * configuration.
 *
 * As of now the BitmapDecompressor supports reading from RGBA_8 bitmaps
 * only. When requesting RGB_8 scanlines, the alpha information is silently
 * dropped.
 */
class BitmapDecompressor final : public IDecompressor {
 public:
  /**
   * Creates a new bitmap decompressor.
   *
   * @param source the source to decompress the image from.
   * @param samplingRatio the sampling ratio to use.
   */
  explicit BitmapDecompressor(
      io::IImageSource& source,
      const folly::Optional<image::Ratio>& samplingRatio = folly::none);

  BitmapDecompressor(BitmapDecompressor&&) = default;

  ~BitmapDecompressor() override = default;

  image::Specification sourceImageSpecification() override;
  image::Specification outputImageSpecification() override;

  std::unique_ptr<image::Scanline> readScanline() override;

  static DecompressorProvider makeProvider();

 private:
  io::IBitmapImageSource& _source;
  std::size_t _currentLine{0};
};

} // namespace bitmap
} // namespace codecs
} // namespace spectrum
} // namespace facebook
