// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/Constants.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/plugins/jpeg/LibJpegSinkManager.h>
#include <spectrum/plugins/jpeg/LibJpegSourceManager.h>
#include <spectrum/requirements/Crop.h>
#include <spectrum/requirements/Rotate.h>

#include <array>
#include <memory>

#include <mozjpeg/jerror.h>
#include <mozjpeg/jinclude.h>
#include <mozjpeg/jpeglib.h>

#include <folly/Optional.h>

extern "C" {
#include <mozjpeg/transupp.h> // doesn't play nice with C++
}

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

/**
 * The LibJpegDctTransformer uses the transupp.h abilities of libjpeg/mozjpeg to
 * perform lossless transformations on a JPEG image. It supports rotation and
 * cropping.
 */
class LibJpegDctTransformer {
 public:
  explicit LibJpegDctTransformer(
      io::IImageSource& source,
      io::IImageSink& sink);

  LibJpegDctTransformer(const LibJpegDctTransformer&) = delete;
  LibJpegDctTransformer(LibJpegDctTransformer&&) = default;

  ~LibJpegDctTransformer();

  /**
   * Applies the given rotation (0, 90, 180 or 270 degrees) losslessly to the
   * DCT coefficients
   */
  void setRotateRequirement(
      const folly::Optional<requirements::Rotate>& rotateRequirement);

  /**
   * Applies the given cropping with absolute values losslessly to the DCT
   * cofficients. The result might differ from the exact size by +/- MCU
   * size (usually 16)
   */
  void setCropRequirement(
      const folly::Optional<requirements::Crop>& cropRequirement);

  /**
   * Executes the transformation that have been previously set to this method.
   * It will read from the image source and write to the image sink. After
   * calling this method, no other method of the `LibJpegDctTransformer` shall
   * be called.
   */
  void applyAndFinish();

  /**
   * Returns the exact size of the resulting JPEG file. Must only be
   * called after `applyAndFinish`.
   */
  image::Size getOutputSize() const;

 private:
  LibJpegSourceManager sourceManager;
  LibJpegSinkManager sinkManager;

  jpeg_decompress_struct libJpegDecompressInfo = {};
  jpeg_transform_info libJpegTransformInfo = {};
  jpeg_compress_struct libJpegCompressInfo = {};
  jpeg_error_mgr libJpegErrorManager = {};

  folly::Optional<requirements::Rotate> rotateRequirement;
  folly::Optional<requirements::Crop> cropRequirement;

  void ensureHeaderIsRead();

  bool isFinished = false;
  void ensureNotFinished();

  void applyRotationToTransformInfo();
  void applyCroppingToTransformInfo();
};

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
