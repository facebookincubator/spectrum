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

#include <folly/Optional.h>
#include "png.h"

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace png {

/**
 * LibPngDecompressor is a wrapper around libpng compatible ABIs and the
 * png_structp. It manages the underlying resource, handles the
 * conversion to Spectrum types and helps to prevent allocation and logic
 * errors.
 */
class LibPngDecompressor final : public codecs::IDecompressor {
 public:
  /**
   * Creates a new png decompressor.
   *
   * @param source The source to decompress the image from.
   * @param samplingRatio the sampling ratio to use.
   */
  explicit LibPngDecompressor(
      io::IImageSource& source,
      const folly::Optional<image::Ratio>& samplingRatio = folly::none);

  LibPngDecompressor(LibPngDecompressor&&) = default;

  ~LibPngDecompressor() override;

 private:
  struct LibPngDecompressorErrorHandler;

  png_structp libPngReadStruct;
  png_infop libPngInfoStruct;
  png_infop libPngEndInfoStruct;

  io::IImageSource& source;

  folly::Optional<image::Specification> _sourceImageSpecification;

  bool isHeaderRead = false;
  bool isInterlaced = false;
  std::size_t outputScanline = 0;

  folly::Optional<std::string> errorMessage;

  void ensureHeaderIsRead();
  void ensureReadyToReadScanline();

  void setErrorMessage(const std::string& errorMessage);
  void throwError(
      const char* const function,
      const unsigned int line,
      const char* const culprit);

  std::unique_ptr<image::Scanline> readOneLine();

  void ensureEntireImageIsRead(
      std::vector<std::unique_ptr<image::Scanline>>* imageVector);
  std::vector<std::unique_ptr<image::Scanline>> entireImage;

  //
  // Decompressor
  //
 public:
  image::Specification sourceImageSpecification() override;
  image::Specification outputImageSpecification() override;

  std::unique_ptr<image::Scanline> readScanline() override;
};

} // namespace png
} // namespace plugins
} // namespace spectrum
} // namespace facebook
