// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "VectorImageSource.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSource.h>

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace facebook {
namespace spectrum {
namespace io {

//
// VectorImageSourceHandler
//

VectorImageSourceHandler::VectorImageSourceHandler(std::vector<char> data)
    : _data(std::move(data)) {}

std::size_t VectorImageSourceHandler::_getTotalBytesRead() const {
  return _offset;
}

std::size_t VectorImageSourceHandler::_read(
    char* const destination,
    const std::size_t length) {
  const auto bytesLeft = _data.size() - _offset;
  const auto bytesToRead = std::min(length, bytesLeft);

  std::copy_n(_data.data() + _offset, bytesToRead, destination);
  _offset += bytesToRead;

  return bytesToRead;
}

std::size_t VectorImageSourceHandler::_available() {
  return _data.size() - _offset;
}

//
// VectorBitmapImageSource
//

VectorBitmapImageSource::VectorBitmapImageSource(
    std::vector<char> data,
    const image::Specification& imageSpecification)
    : IBitmapImageSource(),
      _delegate(std::move(data)),
      _imageSpecification(imageSpecification) {}

image::Specification VectorBitmapImageSource::imageSpecification() const {
  return _imageSpecification;
}

std::size_t VectorBitmapImageSource::read(
    char* const destination,
    const std::size_t length) {
  return _delegate._read(destination, length);
}

std::size_t VectorBitmapImageSource::getTotalBytesRead() const {
  return _delegate._getTotalBytesRead();
}

std::size_t VectorBitmapImageSource::available() {
  return _delegate._available();
}

//
// VectorEncodedImageSource
//

VectorEncodedImageSource::VectorEncodedImageSource(std::vector<char> data)
    : IEncodedImageSource(), _delegate(std::move(data)) {}

std::size_t VectorEncodedImageSource::read(
    char* const destination,
    const std::size_t length) {
  return _delegate._read(destination, length);
}

std::size_t VectorEncodedImageSource::getTotalBytesRead() const {
  return _delegate._getTotalBytesRead();
}

std::size_t VectorEncodedImageSource::available() {
  return _delegate._available();
}

} // namespace io
} // namespace spectrum
} // namespace facebook
