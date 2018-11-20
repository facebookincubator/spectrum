// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/codecs/IDecompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/core/DataRange.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/image/Specification.h>
#include <spectrum/image/metadata/Entries.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/plugins/jpeg/LibJpegSourceManager.h>

#include <array>
#include <memory>
#include <tuple>

#include <mozjpeg/jerror.h>
#include <mozjpeg/jinclude.h>
#include <mozjpeg/jpeglib.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

constexpr int LIBJPEG_SCALE_DENOMINATOR = 8;
constexpr int LIBJPEG_SCALE_NUMERATOR_MIN = 1;
constexpr int LIBJPEG_SCALE_NUMERATOR_MAX = 16;
constexpr auto LIBJPEG_SCALE_DEFAULT =
    image::Ratio{LIBJPEG_SCALE_DENOMINATOR, LIBJPEG_SCALE_DENOMINATOR};

/**
 * LibJpegDecompressor is a wrapper around libjpeg compatible ABIs and the
 * jpeg_decompress_struct. It manages the underlying resource, handles the
 * conversion to Spectrum types and helps to prevent allocation and logic
 * errors.
 */
class LibJpegDecompressor final : public codecs::IDecompressor {
 public:
  /**
   * Creates a new jpeg decompressor.

   * @param source The source to decompress the image from.
   * @param configuration The configuration to use.
   * @param samplingRatio The sampling ratio to use.
   * @param overridePixelSpecification The pixel specification to decompress to.
   */
  explicit LibJpegDecompressor(
      io::IImageSource& source,
      const Configuration& configuration = Configuration(),
      const folly::Optional<image::Ratio>& samplingRatio = folly::none,
      const folly::Optional<image::pixel::Specification>&
          overridePixelSpecification = folly::none);

  LibJpegDecompressor(LibJpegDecompressor&&) = default;

  ~LibJpegDecompressor() override;

 private:
  const Configuration _configuration;
  LibJpegSourceManager sourceManager;

  folly::Optional<image::Specification> _sourceImageSpecification;
  folly::Optional<image::Specification> _outputImageSpecification;
  folly::Optional<image::pixel::Specification>
      _overrideOutputPixelSpecification;

  jpeg_decompress_struct libJpegDecompressInfo = {};
  jpeg_error_mgr libJpegErrorManager = {};

  const image::Ratio _samplingRatio;
  bool _isFinished{false};

  void ensureHeaderIsRead();
  void ensureReadyForReadScanline();

  image::Specification _imageSpecification(
      const image::Size& size,
      const image::pixel::Specification& pixelSpecification);

  image::ChromaSamplingMode _chromaSamplingMode();

  //
  // Decompressor
  //
 public:
  image::Specification sourceImageSpecification() override;
  image::Specification outputImageSpecification() override;

  std::unique_ptr<image::Scanline> readScanline() override;
};

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
