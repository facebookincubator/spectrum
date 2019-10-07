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

template <class Interface, typename T>
VectorImageSink<Interface, T>::VectorImageSink(std::size_t initialCapacity) {
  _data.reserve(initialCapacity);
}

template <class Interface, typename T>
void VectorImageSink<Interface, T>::setConfiguration(
    const image::Size& imageSize,
    const image::pixel::Specification& pixelSpecification) {
  _imageSize = std::move(imageSize);
  _pixelSpecification = std::move(pixelSpecification);
  _data.reserve(
      _imageSize->width * _imageSize->height *
      _pixelSpecification->bytesPerPixel);
}

template <class Interface, typename T>
void VectorImageSink<Interface, T>::_write(
    const char* const source,
    const std::size_t length) {
  SPECTRUM_ENFORCE_IF_NOT(source != nullptr);
  _data.insert(_data.end(), source, source + length);
}

template class VectorImageSink<IBitmapImageSink, char>;
template class VectorImageSink<IBitmapImageSink, uint8_t>;
template class VectorImageSink<IEncodedImageSink, char>;
template class VectorImageSink<IEncodedImageSink, uint8_t>;

} // namespace io
} // namespace spectrum
} // namespace facebook
