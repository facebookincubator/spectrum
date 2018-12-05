// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/codecs/CompressorProvider.h>
#include <spectrum/codecs/ICompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSink.h>

#include "png.h"

#include <array>
#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace png {

/**
 * The LibPngCompressor is a wrapper around libpng compatible ABIs.
 * It manages the underlying resource, handles conversion to Spectrum types
 * and helps to prevent allocation and logic errors.
 */
class LibPngCompressor final : public codecs::ICompressor {
 public:
  /**
   * Creates a new png compressor.
   *
   * @param options The compressor options to use.
   */
  explicit LibPngCompressor(const codecs::CompressorOptions& options);

  LibPngCompressor(LibPngCompressor&&) = default;

  ~LibPngCompressor() override;

 private:
  struct LibPngCompressorErrorHandler;

  codecs::CompressorOptions _options;

  png_structp libPngWriteStruct;
  png_infop libPngInfoStruct;
  std::size_t inputScanline = 0;
  bool isHeaderWritten = false;
  bool writtenLastScanline = false;

  /**
   * For interlaced output all scanlines need to be buffered before writing them
   * all at once
   */
  std::vector<std::unique_ptr<image::Scanline>> interlaceScanlineBuffer;

  folly::Optional<std::string> errorMessage;

  void ensureHeaderIsWritten(std::uint16_t colorType);
  void finishIfLastScanlineWritten();

  void internalWriteScanlineBaseline(std::unique_ptr<image::Scanline> scanline);
  void internalWriteScanlineInterlaced(
      std::unique_ptr<image::Scanline> scanline);

  void setErrorMessage(const std::string& errorMessage);
  void throwError(
      const char* const function,
      const unsigned int line,
      const char* const culprit);

  //
  // Compressor
  //
 public:
  void writeScanline(std::unique_ptr<image::Scanline> scanline) override;
};

} // namespace png
} // namespace plugins
} // namespace spectrum
} // namespace facebook
