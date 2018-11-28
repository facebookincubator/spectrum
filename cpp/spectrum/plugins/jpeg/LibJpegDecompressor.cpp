// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegDecompressor.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/plugins/jpeg/LibJpegUtilities.h>

#include <mozjpeg/jerror.h>
#include <mozjpeg/jinclude.h>
#include <mozjpeg/jpeglib.h>

#include <mozjpeg/jpegint.h>

#include <exception>
#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

namespace /* anonymous */ {
void libJpegErrorToRuntimeExecption(j_common_ptr cinfo) {
  // create and throw the jpeg-turbo error message
  char buffer[JMSG_LENGTH_MAX];
  (*cinfo->err->format_message)(cinfo, buffer);

  buffer[JMSG_LENGTH_MAX - 1] = '\0';
  SPECTRUM_ERROR_STRING(
      codecs::error::DecompressorFailure, std::string(buffer));
}

image::pixel::Specification _pixelSpecificationFromColorSpace(
    J_COLOR_SPACE colorSpace,
    int componentsCount) {
  if (colorSpace == JCS_GRAYSCALE && componentsCount == 1) {
    return image::pixel::specifications::Gray;
  } else if (
      (colorSpace == JCS_RGB || colorSpace == JCS_EXT_RGB) &&
      componentsCount == 3) {
    return image::pixel::specifications::RGB;
  } else if (colorSpace == JCS_YCbCr && componentsCount == 3) {
    return image::pixel::specifications::yCbCr;
  } else {
    SPECTRUM_ERROR_FORMAT(
        codecs::error::DecompressorUnexpectedPixelSpecification,
        "colorSpace: %d, components: %d",
        colorSpace,
        componentsCount);
  }
}
} // namespace

LibJpegDecompressor::LibJpegDecompressor(
    io::IImageSource& source,
    const Configuration& configuration,
    const folly::Optional<image::Ratio>& samplingRatio,
    const folly::Optional<image::pixel::Specification>&
        overridePixelSpecification)
    : _configuration(configuration),
      sourceManager(source),
      _overrideOutputPixelSpecification(overridePixelSpecification),
      _samplingRatio(samplingRatio.value_or(LIBJPEG_SCALE_DEFAULT)) {
  SPECTRUM_ENFORCE_IF(_samplingRatio.numerator < LIBJPEG_SCALE_NUMERATOR_MIN);
  SPECTRUM_ENFORCE_IF(_samplingRatio.numerator > LIBJPEG_SCALE_NUMERATOR_MAX);
  SPECTRUM_ENFORCE_IF_NOT(
      _samplingRatio.denominator == LIBJPEG_SCALE_DENOMINATOR);

  // Error handling
  libJpegDecompressInfo.err = jpeg_std_error(&libJpegErrorManager);
  libJpegErrorManager.error_exit = libJpegErrorToRuntimeExecption;

  jpeg_create_decompress(&libJpegDecompressInfo);

  // Source manager
  libJpegDecompressInfo.src = sourceManager.getLibJpegSourceManagerPointer();
};

LibJpegDecompressor::~LibJpegDecompressor() {
  jpeg_destroy_decompress(&libJpegDecompressInfo);
}

image::ChromaSamplingMode LibJpegDecompressor::_chromaSamplingMode() {
  ensureHeaderIsRead();

  if (libJpegDecompressInfo.max_h_samp_factor == 1 &&
      libJpegDecompressInfo.max_v_samp_factor == 1) {
    return image::ChromaSamplingMode::S444;
  } else if (
      libJpegDecompressInfo.max_h_samp_factor == 2 &&
      libJpegDecompressInfo.max_v_samp_factor == 2) {
    return image::ChromaSamplingMode::S420;
  } else if (
      libJpegDecompressInfo.max_h_samp_factor == 2 &&
      libJpegDecompressInfo.max_v_samp_factor == 1) {
    return image::ChromaSamplingMode::S422;
  } else if (
      libJpegDecompressInfo.max_h_samp_factor == 4 &&
      libJpegDecompressInfo.max_v_samp_factor == 1) {
    return image::ChromaSamplingMode::S411;
  } else if (
      libJpegDecompressInfo.max_h_samp_factor == 1 &&
      libJpegDecompressInfo.max_v_samp_factor == 2) {
    return image::ChromaSamplingMode::S440;
  } else {
    SPECTRUM_ERROR(codecs::error::DecompressorUnexpectedChromaSamplingMode);
  }
}

//
// Private
//

void LibJpegDecompressor::ensureHeaderIsRead() {
  if (libJpegDecompressInfo.global_state < DSTATE_INHEADER) {
    if (_configuration.general.interpretMetadata()) {
      saveMetadataMarkers(libJpegDecompressInfo);
    }

    const auto result = jpeg_read_header(&libJpegDecompressInfo, true);
    SPECTRUM_ERROR_CSTR_IF_NOT(
        JPEG_HEADER_OK == result,
        codecs::error::DecompressorFailure,
        "jpeg_read_header_failed");
  }

  // must be executed between reading the header and starting the
  // decompression
  libJpegDecompressInfo.scale_num = _samplingRatio.numerator;
  libJpegDecompressInfo.scale_denom = _samplingRatio.denominator;

  // if we override the pixel type, then we'll want to set it at this point
  // in the decompress struct to allow libjpeg initialize correctly
  if (_overrideOutputPixelSpecification.hasValue()) {
    const auto colorModel =
        _overrideOutputPixelSpecification.value().colorModel;
    if (colorModel == image::pixel::colormodels::Gray) {
      libJpegDecompressInfo.out_color_space = JCS_GRAYSCALE;
    } else if (colorModel == image::pixel::colormodels::RGB) {
      libJpegDecompressInfo.out_color_space = JCS_RGB;
    } else {
      SPECTRUM_ERROR_STRING(
          codecs::error::DecompressorUnsupportedPixelSpecificationOverride,
          colorModel.identifier.toStdString());
    }
  }
}

void LibJpegDecompressor::ensureReadyForReadScanline() {
  ensureHeaderIsRead();

  if (libJpegDecompressInfo.global_state < DSTATE_SCANNING) {
    jpeg_start_decompress(&libJpegDecompressInfo);
    SPECTRUM_ERROR_CSTR_IF_NOT(
        libJpegDecompressInfo.global_state >= DSTATE_SCANNING,
        codecs::error::DecompressorFailure,
        "jpeg_start_decompress_failed");
  }

  SPECTRUM_ENFORCE_IF_NOT(
      libJpegDecompressInfo.output_scanline <
      libJpegDecompressInfo.output_height);
}

image::Specification LibJpegDecompressor::_imageSpecification(
    const image::Size& size,
    const image::pixel::Specification& pixelSpecification) {
  ensureHeaderIsRead();

  const auto metadata = readMetadata(libJpegDecompressInfo);
  const auto orientation =
      metadata.entries().orientation().value_or(image::Orientation::Up);

  return image::Specification{
      .size = size,
      .format = image::formats::Jpeg,
      .pixelSpecification = pixelSpecification,
      .orientation = orientation,
      .chromaSamplingMode = _chromaSamplingMode(),
      .metadata = metadata,
  };
}

std::unique_ptr<image::Scanline> LibJpegDecompressor::readScanline() {
  ensureHeaderIsRead();
  ensureReadyForReadScanline();

  const auto pixelSpecification = outputImageSpecification().pixelSpecification;

  auto result = std::make_unique<image::Scanline>(
      pixelSpecification, libJpegDecompressInfo.output_width);
  JSAMPROW scanlineDest = reinterpret_cast<JSAMPROW>(result->data());

  jpeg_read_scanlines(
      &libJpegDecompressInfo,
      reinterpret_cast<JSAMPARRAY>(
          &scanlineDest), /* pointer to array of pointers */
      1 /* number of scanlines to read */);

  // free memory after last line has been read
  if (libJpegDecompressInfo.output_scanline >=
      libJpegDecompressInfo.output_height) {
    // T29725613: for malformed images without EOI, the jpeg_finish_decompress()
    // method might fail with an error. We don't care about markers or anything
    // after the image has already been read. Therefore, we do the deallocation
    // and terminate the source ourselves. See jdapimin.c
    (libJpegDecompressInfo.src->term_source)(&libJpegDecompressInfo);
    jpeg_abort((j_common_ptr)&libJpegDecompressInfo);
    _isFinished = true;
  }

  return result;
}

//
// Decompressor
//

image::Specification LibJpegDecompressor::sourceImageSpecification() {
  if (_sourceImageSpecification.hasValue()) {
    return *_sourceImageSpecification;
  }

  ensureReadyForReadScanline();
  const auto inputSize = image::Size{

      .width = SPECTRUM_CONVERT_OR_THROW(
          libJpegDecompressInfo.image_width, std::uint32_t),
      .height = SPECTRUM_CONVERT_OR_THROW(
          libJpegDecompressInfo.image_height, std::uint32_t),
  };

  const auto inputPixelSpecification = _pixelSpecificationFromColorSpace(
      libJpegDecompressInfo.jpeg_color_space,
      libJpegDecompressInfo.num_components);

  _sourceImageSpecification =
      _imageSpecification(inputSize, inputPixelSpecification);
  return *_sourceImageSpecification;
}

image::Specification LibJpegDecompressor::outputImageSpecification() {
  if (_outputImageSpecification.hasValue()) {
    return *_outputImageSpecification;
  }

  // Needed for output_width / output_height after setting the sampling ratio
  ensureReadyForReadScanline();

  const auto outputSize = image::Size{
      .width = SPECTRUM_CONVERT_OR_THROW(
          libJpegDecompressInfo.output_width, std::uint32_t),
      .height = SPECTRUM_CONVERT_OR_THROW(
          libJpegDecompressInfo.output_height, std::uint32_t),
  };
  const auto outputPixelSpecification = _pixelSpecificationFromColorSpace(
      libJpegDecompressInfo.out_color_space,
      libJpegDecompressInfo.out_color_components);

  _outputImageSpecification =
      _imageSpecification(outputSize, outputPixelSpecification);
  return *_outputImageSpecification;
}

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
