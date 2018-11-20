// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "BitmapDecompressor.h"

#include <spectrum/codecs/IDecompressor.h>
#include <spectrum/core/Constants.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSource.h>

#include <array>
#include <memory>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace bitmap {

BitmapDecompressor::BitmapDecompressor(
    io::IImageSource& source,
    const folly::Optional<image::Ratio>& samplingRatio)
    : _source(reinterpret_cast<io::IBitmapImageSource&>(source)) {
  IDecompressor::_ensureNoSamplingRatio(samplingRatio);
}

std::unique_ptr<image::Scanline> BitmapDecompressor::readScanline() {
  const auto imageSpecification = _source.imageSpecification();
  SPECTRUM_ENFORCE_IF_NOT(_currentLine < imageSpecification.size.height);

  auto inputScanline = std::make_unique<image::Scanline>(
      imageSpecification.pixelSpecification, imageSpecification.size.width);
  const std::size_t widthBytes = imageSpecification.size.width *
      imageSpecification.pixelSpecification.bytesPerPixel;
  const std::size_t numBytesRead =
      _source.read(reinterpret_cast<char*>(inputScanline->data()), widthBytes);

  SPECTRUM_ENFORCE_IF_NOT(numBytesRead == widthBytes);

  _currentLine++;

  return inputScanline;
}

image::Specification BitmapDecompressor::sourceImageSpecification() {
  return _source.imageSpecification();
}

image::Specification BitmapDecompressor::outputImageSpecification() {
  return _source.imageSpecification();
}

DecompressorProvider BitmapDecompressor::makeProvider() {
  return DecompressorProvider{
      .format = image::formats::Bitmap,
      .decompressorFactory =
          [](io::IImageSource& source,
             const folly::Optional<image::Ratio>& samplingRatio,
             const Configuration& /* unused */) {
            return std::make_unique<BitmapDecompressor>(source, samplingRatio);
          },
  };
}

} // namespace bitmap
} // namespace codecs
} // namespace spectrum
} // namespace facebook
