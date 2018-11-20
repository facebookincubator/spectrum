// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/CompressorProvider.h>
#include <spectrum/codecs/ICompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/image/Metadata.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSink.h>

#include <array>
#include <memory>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace bitmap {

/**
 * The BitmapCompressor will write the scanlines directly into the output stream
 * such that the binding can feed them into a native bitmap that stores pixel
 * values.
 */
class BitmapCompressor final : public ICompressor {
 public:
  /**
   * Creates a new bitmap compressor.
   *
   * @param options The compressor options to use.
   */
  explicit BitmapCompressor(const CompressorOptions& options);

  BitmapCompressor(BitmapCompressor&&) = default;

  ~BitmapCompressor() override = default;

  void writeScanline(std::unique_ptr<image::Scanline> scanline) override;

  static CompressorProvider makeProvider();

 private:
  CompressorOptions _options;
  std::uint32_t _nextScanline{0};
};

} // namespace bitmap
} // namespace codecs
} // namespace spectrum
} // namespace facebook
