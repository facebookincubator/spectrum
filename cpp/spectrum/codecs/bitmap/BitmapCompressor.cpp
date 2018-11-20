// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "BitmapCompressor.h"

#include <spectrum/codecs/ICompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSink.h>

#include <array>
#include <functional>
#include <memory>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace bitmap {

BitmapCompressor::BitmapCompressor(const CompressorOptions& options)
    : _options(options) {
  ICompressor::enforcePixelSpecificationSet(
      options.imageSpecification.pixelSpecification);
  ICompressor::enforceCannotEncodeMetadata(options.imageSpecification.metadata);
  ICompressor::enforceLossless(options.encodeRequirement);

  options.sink.setConfiguration(
      options.imageSpecification.size,
      options.imageSpecification.pixelSpecification);
}

//
// Compressor
//

void BitmapCompressor::writeScanline(
    std::unique_ptr<image::Scanline> scanline) {
  SPECTRUM_ENFORCE_IF_NOT(
      scanline->specification() ==
      _options.imageSpecification.pixelSpecification);
  SPECTRUM_ENFORCE_IF_NOT(
      _nextScanline < _options.imageSpecification.size.height);
  SPECTRUM_ENFORCE_IF_NOT(
      scanline->width() == _options.imageSpecification.size.width);

  const std::size_t sizeBytes = scanline->sizeBytes();
  _options.sink.write(reinterpret_cast<char*>(scanline->data()), sizeBytes);

  _nextScanline++;
}

CompressorProvider BitmapCompressor::makeProvider() {
  return CompressorProvider{
      .format = image::formats::Bitmap,
      .supportsSettingMetadata = false,
      .pixelSpecificationNarrower = nullptr,
      .supportedChromaSamplingModes =
          {
              image::ChromaSamplingMode::S444,
          },
      .compressorFactory =
          [](const CompressorOptions& options) {
            return std::make_unique<BitmapCompressor>(options);
          },
  };
}

} // namespace bitmap
} // namespace codecs
} // namespace spectrum
} // namespace facebook
