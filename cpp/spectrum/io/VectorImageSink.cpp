// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "VectorImageSink.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSource.h>

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace facebook {
namespace spectrum {
namespace io {

VectorBitmapImageSink::VectorBitmapImageSink(const std::size_t initialCapacity)
    : IBitmapImageSink() {
  _data.reserve(initialCapacity);
}

void VectorBitmapImageSink::_write(
    const char* const source,
    const std::size_t length) {
  SPECTRUM_ENFORCE_IF_NOT(source != nullptr);
  _data.insert(_data.end(), source, source + length);
}

void VectorBitmapImageSink::setConfiguration(
    const image::Size& imageSize,
    const image::pixel::Specification& pixelSpecification) {
  _imageSize = imageSize;
  _pixelSpecification = pixelSpecification;
  const std::size_t sizePerPixel = pixelSpecification.bytesPerPixel;
  const std::size_t totalNumPixel = imageSize.width * imageSize.height;
  const std::size_t totalSize = totalNumPixel * sizePerPixel;

  _data.reserve(totalSize);
}

} // namespace io
} // namespace spectrum
} // namespace facebook
