// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "VectorImageSink.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace facebook {
namespace spectrum {
namespace io {

template <class Interface>
VectorImageSink<Interface>::VectorImageSink(std::size_t initialCapacity) {
  _data.reserve(initialCapacity);
}

template <class Interface>
void VectorImageSink<Interface>::setConfiguration(
    const image::Size& imageSize,
    const image::pixel::Specification& pixelSpecification) {
  _imageSize = std::move(imageSize);
  _pixelSpecification = std::move(pixelSpecification);
  _data.reserve(
      _imageSize->width * _imageSize->height *
      _pixelSpecification->bytesPerPixel);
}

template <class Interface>
void VectorImageSink<Interface>::_write(
    const char* const source,
    const std::size_t length) {
  SPECTRUM_ENFORCE_IF_NOT(source != nullptr);
  _data.insert(_data.end(), source, source + length);
}

template class VectorImageSink<IBitmapImageSink>;
template class VectorImageSink<IEncodedImageSink>;

} // namespace io
} // namespace spectrum
} // namespace facebook
