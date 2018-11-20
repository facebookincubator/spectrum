// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegCompressor.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/plugins/jpeg/LibJpegConstants.h>
#include <spectrum/plugins/jpeg/LibJpegUtilities.h>

#include <exception>
#include <memory>

#include <mozjpeg/jerror.h>
#include <mozjpeg/jinclude.h>
#include <mozjpeg/jpeglib.h>

#include <mozjpeg/jpegint.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

constexpr requirements::Encode::Quality LibJpegCompressor::QualityDefault;
constexpr requirements::Encode::Quality LibJpegCompressor::QualityMin;
constexpr requirements::Encode::Quality LibJpegCompressor::QualityMax;

namespace /* anonymous */ {
void libJpegErrorToRuntimeExecption(j_common_ptr cinfo) {
  // create and throw the jpeg-turbo error message
  char buffer[JMSG_LENGTH_MAX];
  (*cinfo->err->format_message)(cinfo, buffer);

  buffer[JMSG_LENGTH_MAX - 1] = '\0';
  SPECTRUM_ERROR_STRING(codecs::error::CompressorFailure, std::string(buffer));
}
} // namespace

LibJpegCompressor::LibJpegCompressor(const codecs::CompressorOptions& options)
    : quality(ICompressor::_sanitizedQualityWithDefault(
          options.encodeRequirement,
          QualityDefault,
          QualityMin,
          QualityMax)),
      _options(options),
      sinkManager(options.sink) {
  ICompressor::enforceLossy(options.encodeRequirement);
  ICompressor::enforceSizeBelowMaximumSideDimension(
      options.imageSpecification.size, maximumSizeDimension);

  // set error handling
  libJpegCompressInfo.err = jpeg_std_error(&libJpegErrorManager);
  libJpegErrorManager.error_exit = libJpegErrorToRuntimeExecption;

  // initialize decompress struct
  jpeg_create_compress(&libJpegCompressInfo);

  // setup pixel type
  const auto pixelSpecification =
      _options.imageSpecification.pixelSpecification;
  if (pixelSpecification == image::pixel::specifications::Gray) {
    libJpegCompressInfo.input_components = 1;
    libJpegCompressInfo.in_color_space = JCS_GRAYSCALE;
  } else if (pixelSpecification == image::pixel::specifications::RGB) {
    libJpegCompressInfo.input_components = 3;
    libJpegCompressInfo.in_color_space = JCS_RGB;
  } else {
    SPECTRUM_ERROR_STRING(
        codecs::error::CompressorCannotWritePixelSpecification,
        pixelSpecification.string());
  }

  // set image dimenstions
  const auto imageSize = options.imageSpecification.size;
  libJpegCompressInfo.image_width = imageSize.width;
  libJpegCompressInfo.image_height = imageSize.height;

  // propagate image configuration
  options.sink.setConfiguration(
      imageSize, options.imageSpecification.pixelSpecification);

  // set remaining settings to libJpeg's default
  jpeg_set_defaults(&libJpegCompressInfo);

  // register sink manager
  libJpegCompressInfo.dest = sinkManager.getLibJpegDestinationManagerPointer();
}

LibJpegCompressor::~LibJpegCompressor() {
  jpeg_destroy_compress(&libJpegCompressInfo);
}

//
// Private
//

void LibJpegCompressor::ensureBeforeCompressionStarted() {
  SPECTRUM_ENFORCE_IF_NOT(libJpegCompressInfo.global_state == CSTATE_START);
}

void LibJpegCompressor::ensureReadyForWriteScanline() {
  if (libJpegCompressInfo.global_state < CSTATE_SCANNING) {
    jpeg_c_set_bool_param(
        &libJpegCompressInfo,
        JBOOLEAN_TRELLIS_QUANT,
        _options.configuration.jpeg.useTrellis());
    jpeg_c_set_bool_param(
        &libJpegCompressInfo,
        JBOOLEAN_OPTIMIZE_SCANS,
        _options.configuration.jpeg.useOptimizeScan());

    if (_options.configuration.jpeg.useCompatibleDcScanOpt()) {
      jpeg_c_set_int_param(&libJpegCompressInfo, JINT_DC_SCAN_OPT_MODE, 0);
    }

    if (_options.imageSpecification.chromaSamplingMode ==
        image::ChromaSamplingMode::S444) {
      // Add support for 422?
      for (int i = 0; i < MAX_COMPONENTS; i++) {
        // 4:4:4 sampling translates into H/V sample factors of 1
        libJpegCompressInfo.comp_info[i].h_samp_factor = 1;
        libJpegCompressInfo.comp_info[i].v_samp_factor = 1;
      }
    }

    if (_options.configuration.jpeg.usePsnrQuantTable()) {
      // similar to cjpeg.c
      jpeg_c_set_int_param(&libJpegCompressInfo, JINT_BASE_QUANT_TBL_IDX, 1);
      jpeg_c_set_float_param(
          &libJpegCompressInfo, JFLOAT_LAMBDA_LOG_SCALE1, 9.0);
      jpeg_c_set_float_param(
          &libJpegCompressInfo, JFLOAT_LAMBDA_LOG_SCALE2, 0.0);
      jpeg_c_set_bool_param(
          &libJpegCompressInfo, JBOOLEAN_USE_LAMBDA_WEIGHT_TBL, FALSE);
    }

    jpeg_set_quality(&libJpegCompressInfo, quality, false /* force_baseline */);

    if (_options.configuration.jpeg.useProgressive()) {
      // re-setup DC and AC scan layout
      jpeg_simple_progression(&libJpegCompressInfo);
    } else {
      libJpegCompressInfo.num_scans = 0;
      libJpegCompressInfo.scan_info = NULL;
    }

    jpeg_start_compress(&libJpegCompressInfo, true);

    SPECTRUM_ERROR_CSTR_IF_NOT(
        libJpegCompressInfo.global_state >= CSTATE_SCANNING,
        codecs::error::CompressorFailure,
        "jpeg_start_compress_failed");

    if (_options.configuration.general.interpretMetadata() &&
        !_options.imageSpecification.metadata.empty()) {
      writeMetadata(libJpegCompressInfo, _options.imageSpecification.metadata);
    }
  }

  SPECTRUM_ENFORCE_IF_NOT(!writtenLastScanline);
}

void LibJpegCompressor::finishIfLastScanlineWritten() {
  if (libJpegCompressInfo.next_scanline == libJpegCompressInfo.image_height) {
    writtenLastScanline = true;
    jpeg_finish_compress(&libJpegCompressInfo);
  }
}

void LibJpegCompressor::internalWriteScanline(
    JSAMPROW scanlineData,
    const std::size_t scanlineSize,
    const image::pixel::Specification& pixelSpecification) {
  SPECTRUM_ENFORCE_IF_NOT(
      pixelSpecification == _options.imageSpecification.pixelSpecification);
  SPECTRUM_ENFORCE_IF_NOT(scanlineSize == libJpegCompressInfo.image_width);
  SPECTRUM_ENFORCE_IF(writtenLastScanline);

  ensureReadyForWriteScanline();

  jpeg_write_scanlines(
      &libJpegCompressInfo,
      reinterpret_cast<JSAMPARRAY>(
          &scanlineData), /* pointer to array of pointers */
      1 /* number of scanlines to read */);

  finishIfLastScanlineWritten();
}

//
// Compressor
//

void LibJpegCompressor::writeScanline(
    std::unique_ptr<image::Scanline> scanline) {
  const auto pixelSpecification = scanline->specification();
  if (pixelSpecification == image::pixel::specifications::Gray ||
      pixelSpecification == image::pixel::specifications::RGB) {
    internalWriteScanline(
        reinterpret_cast<JSAMPROW>(scanline->data()),
        scanline->width(),
        pixelSpecification);
  } else {
    SPECTRUM_ERROR_STRING(
        codecs::error::CompressorCannotWritePixelSpecification,
        pixelSpecification.string());
  }
}

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
