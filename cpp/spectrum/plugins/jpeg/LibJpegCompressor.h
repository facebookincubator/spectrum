// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/codecs/CompressorProvider.h>
#include <spectrum/codecs/ICompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/image/Metadata.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/plugins/jpeg/LibJpegSinkManager.h>

#include <array>
#include <memory>

#include <mozjpeg/jerror.h>
#include <mozjpeg/jinclude.h>
#include <mozjpeg/jpeglib.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

/**
 * The LibJpegCompressor is a wrapper around libjpeg compatible ABIs and the
 * jpeg_compress_struct. It manages the underlying resource, handles
 * conversion to Spectrum types and helps to prevent allocation and logic
 * errors.
 */
class LibJpegCompressor final : public codecs::ICompressor {
 public:
  static constexpr requirements::Encode::Quality QualityDefault = 75;
  static constexpr requirements::Encode::Quality QualityMin = 10;
  static constexpr requirements::Encode::Quality QualityMax = 95;

  explicit LibJpegCompressor(const codecs::CompressorOptions& options);

  LibJpegCompressor(LibJpegCompressor&&) = default;

  ~LibJpegCompressor() override;

  const requirements::Encode::Quality quality;

 private:
  const codecs::CompressorOptions _options;
  LibJpegSinkManager sinkManager;

  jpeg_compress_struct libJpegCompressInfo = {};
  jpeg_error_mgr libJpegErrorManager = {};
  bool writtenLastScanline = false;

  void ensureBeforeCompressionStarted();
  void ensureReadyForWriteScanline();
  void finishIfLastScanlineWritten();
  void internalWriteScanline(
      JSAMPROW scanlineData,
      const std::size_t scanlineSize,
      const image::pixel::Specification& expectedPixelSpecification);

  //
  // Compressor
  //
 public:
  void writeScanline(std::unique_ptr<image::Scanline> scanline) override;
};

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
