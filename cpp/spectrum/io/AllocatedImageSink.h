// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/IBitmapImageSink.h>

#include <cstddef>
#include <string>

namespace facebook {
namespace spectrum {
namespace io {

namespace error {
extern const folly::StringPiece BufferOverflowError;
} // namespace error

/*
 * The allocated image sink writes image data to an allocated buffer in memory
 * which is passed in the constructor. The image size in bytes is exposed as
 * well.
 *
 * @throws SpectrumException if there is not enough space allocated in the
 * buffer to store the image.
 */
class AllocatedImageSink : public IBitmapImageSink {
 public:
  AllocatedImageSink(void* allocatedBuffer, std::size_t bufferLength);

  AllocatedImageSink(const AllocatedImageSink&) = delete;
  AllocatedImageSink& operator=(const AllocatedImageSink&) = delete;

  AllocatedImageSink(AllocatedImageSink&&) = default;

  void setConfiguration(
      const image::Size& imageSize,
      const image::pixel::Specification& pixelSpecification) override;

  folly::Optional<image::Size> size() const {
    return _imageSize;
  }

  folly::Optional<image::pixel::Specification> getPixelSpecification() const {
    return _pixelSpecification;
  }

 protected:
  void _write(const char* const source, const std::size_t length) override;

 private:
  void* const _buffer;
  const std::size_t _bufferLength;
  folly::Optional<image::Size> _imageSize;
  folly::Optional<image::pixel::Specification> _pixelSpecification;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
