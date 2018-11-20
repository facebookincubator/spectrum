// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegDctTransformer.h"

#include <spectrum/codecs/ICompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/utils/Numeric.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/plugins/jpeg/LibJpegSinkManager.h>
#include <spectrum/plugins/jpeg/LibJpegSourceManager.h>

#include <array>
#include <memory>

#include <mozjpeg/jpegint.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

namespace /* anonymous */ {
void libJpegErrorToSpectrumExecption(j_common_ptr libJpegCompressInfo) {
  // create and throw the jpeg-turbo error message
  char buffer[JMSG_LENGTH_MAX];

  (*libJpegCompressInfo->err->format_message)(libJpegCompressInfo, buffer);

  buffer[JMSG_LENGTH_MAX - 1] = '\0';

  SPECTRUM_ERROR_STRING(codecs::error::CompressorFailure, std::string(buffer));
}
} // namespace

LibJpegDctTransformer::LibJpegDctTransformer(
    io::IImageSource& source,
    io::IImageSink& sink)
    : sourceManager(source), sinkManager(sink) {
  // set error handling
  libJpegErrorManager.error_exit = libJpegErrorToSpectrumExecption;
  libJpegDecompressInfo.err = jpeg_std_error(&libJpegErrorManager);
  libJpegCompressInfo.err = jpeg_std_error(&libJpegErrorManager);

  // initialize decompress struct
  jpeg_create_decompress(&libJpegDecompressInfo);
  libJpegDecompressInfo.src = sourceManager.getLibJpegSourceManagerPointer();

  // initialize compress struct
  jpeg_create_compress(&libJpegCompressInfo);
  libJpegCompressInfo.dest = sinkManager.getLibJpegDestinationManagerPointer();

  // initialize transform info
  libJpegTransformInfo.perfect = false; // allow partial MCUs
  libJpegTransformInfo.trim = true; // allow trimming partial MCUs
  libJpegTransformInfo.force_grayscale = false;
  libJpegTransformInfo.crop = false;
  libJpegTransformInfo.slow_hflip = false;
}

LibJpegDctTransformer::~LibJpegDctTransformer() {
  jpeg_destroy_compress(&libJpegCompressInfo);
  jpeg_destroy_decompress(&libJpegDecompressInfo);
}

void LibJpegDctTransformer::ensureHeaderIsRead() {
  if (libJpegDecompressInfo.global_state < DSTATE_INHEADER) {
    jcopy_markers_setup(&libJpegDecompressInfo, JCOPYOPT_ALL);

    const auto result = jpeg_read_header(&libJpegDecompressInfo, true);
    SPECTRUM_ERROR_CSTR_IF_NOT(
        JPEG_HEADER_OK == result,
        codecs::error::CompressorFailure,
        "jpeg_read_header_failed");
  }
}

void LibJpegDctTransformer::setRotateRequirement(
    const folly::Optional<requirements::Rotate>& rotateRequirement) {
  ensureNotFinished();
  this->rotateRequirement = rotateRequirement;
}

void LibJpegDctTransformer::setCropRequirement(
    const folly::Optional<requirements::Crop>& cropRequirement) {
  ensureNotFinished();

  SPECTRUM_ENFORCE_IF(
      cropRequirement.hasValue() && cropRequirement->mustBeExact);
  this->cropRequirement = cropRequirement;
}

void LibJpegDctTransformer::applyAndFinish() {
  ensureNotFinished();
  ensureHeaderIsRead();

  // apply rotation and cropping to libJpeg objects
  applyRotationToTransformInfo();
  applyCroppingToTransformInfo();

  // intialize and setup temporary buffers - also verifies arguments
  jtransform_request_workspace(&libJpegDecompressInfo, &libJpegTransformInfo);

  jvirt_barray_ptr* srccoefs = jpeg_read_coefficients(&libJpegDecompressInfo);

  // e.g. quant tables
  jpeg_copy_critical_parameters(&libJpegDecompressInfo, &libJpegCompressInfo);

  jvirt_barray_ptr* dstcoefs = jtransform_adjust_parameters(
      &libJpegDecompressInfo,
      &libJpegCompressInfo,
      srccoefs,
      &libJpegTransformInfo);
  jpeg_write_coefficients(&libJpegCompressInfo, dstcoefs);

  jcopy_markers_execute(
      &libJpegDecompressInfo, &libJpegCompressInfo, JCOPYOPT_ALL);

  // will finally apply the transformations (e.g. rotating in place)
  jtransform_execute_transformation(
      &libJpegDecompressInfo,
      &libJpegCompressInfo,
      srccoefs,
      &libJpegTransformInfo);

  // finish
  jpeg_finish_compress(&libJpegCompressInfo);
  jpeg_finish_decompress(&libJpegDecompressInfo);
  isFinished = true;
}

void LibJpegDctTransformer::applyRotationToTransformInfo() {
  if (rotateRequirement.hasValue()) {
    if (rotateRequirement->sanitisedDegrees() == 90) {
      libJpegTransformInfo.transform = JXFORM_ROT_90;
    } else if (rotateRequirement->sanitisedDegrees() == 180) {
      libJpegTransformInfo.transform = JXFORM_ROT_180;
    } else if (rotateRequirement->sanitisedDegrees() == 270) {
      libJpegTransformInfo.transform = JXFORM_ROT_270;
    } else {
      libJpegTransformInfo.transform = JXFORM_NONE;
    }
  } else {
    libJpegTransformInfo.transform = JXFORM_NONE;
  }
}

void LibJpegDctTransformer::applyCroppingToTransformInfo() {
  // LibJpeg first rotates and then crops. However in Spectrum we have the
  // cropping coordinates refer to the input image. Therefore we need to rotate
  // them in these cases.
  if (cropRequirement.hasValue()) {
    image::Size imageSize{
        static_cast<std::uint32_t>(libJpegDecompressInfo.image_width),
        static_cast<std::uint32_t>(libJpegDecompressInfo.image_height)};

    auto cropRect = image::rectZero;

    if (rotateRequirement.hasValue()) {
      const auto rotatedCropRequirement =
          cropRequirement->rotated(*rotateRequirement, imageSize);
      const auto rotatedImageSize =
          imageSize.rotated(rotateRequirement->sanitisedDegrees());
      cropRect = rotatedCropRequirement.apply(rotatedImageSize);
    } else {
      cropRect = cropRequirement->apply(imageSize);
    }

    libJpegTransformInfo.crop = true;

    libJpegTransformInfo.crop_xoffset_set = JCROP_POS;
    libJpegTransformInfo.crop_yoffset_set = JCROP_POS;
    libJpegTransformInfo.crop_width_set = JCROP_POS;
    libJpegTransformInfo.crop_height_set = JCROP_POS;
    libJpegTransformInfo.crop_xoffset = cropRect.topLeft.x;
    libJpegTransformInfo.crop_yoffset = cropRect.topLeft.y;
    libJpegTransformInfo.crop_width = cropRect.size.width;
    libJpegTransformInfo.crop_height = cropRect.size.height;
  }
}

image::Size LibJpegDctTransformer::getOutputSize() const {
  SPECTRUM_ENFORCE_IF_NOT(isFinished);
  return image::Size{(std::uint32_t)libJpegTransformInfo.output_width,
                     (std::uint32_t)libJpegTransformInfo.output_height};
}

void LibJpegDctTransformer::ensureNotFinished() {
  SPECTRUM_ENFORCE_IF(isFinished);
}

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
