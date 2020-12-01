// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "AllocatedImageSink.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace facebook {
namespace spectrum {
namespace io {

namespace error {
const folly::StringPiece BufferOverflowError{
    "allocated_image_sink_buffer_overflow"};
} // namespace error

AllocatedImageSink::AllocatedImageSink(
    void* allocatedBuffer,
    std::size_t bufferLength)
    : _buffer(allocatedBuffer), _bufferLength(bufferLength) {
  SPECTRUM_ENFORCE_IF_NOT(allocatedBuffer != nullptr);
}

void AllocatedImageSink::setConfiguration(
    const image::Size& imageSize,
    const image::pixel::Specification& pixelSpecification) {
  _imageSize = std::move(imageSize);
  _pixelSpecification = std::move(pixelSpecification);
}

void AllocatedImageSink::_write(
    const char* const source,
    const std::size_t length) {
  SPECTRUM_ENFORCE_IF_NOT(source != nullptr);
  auto size = totalBytesWritten();
  SPECTRUM_ERROR_IF(size + length > _bufferLength, error::BufferOverflowError);
  memcpy(static_cast<char*>(_buffer) + size, source, length);
  size += length;
}

} // namespace io
} // namespace spectrum
} // namespace facebook
