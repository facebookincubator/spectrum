// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Specification.h>

#include <folly/Range.h>

#include <cstddef>

namespace facebook {
namespace spectrum {
namespace io {

namespace error {
extern const folly::StringPiece ImageSinkFailure;
}

/**
 * This top level interface abstracts the ability to write continious chunks of
 * data.
 *
 * `IImageSink` should not be subclassed directly. Instead, see
 * `IBitmapImageSink` and `IEncodedImageSink`.
 */
class IImageSink {
 public:
  virtual ~IImageSink() = default;

  /**
   * Optional call to the image sink that sets the configuration of the output
   * such as image size and pixel type.
   *
   * This method might be called more than once. However, if it is called, it is
   * always called with the same parameters and before the first `write` call.
   */
  virtual void setConfiguration(
      const image::Size& imageSize,
      const image::pixel::Specification& pixelSpecification) = 0;

  /**
   * Writes len bytes from *src to this sink.
   *
   * If the sink cannot write the entire len bytes, it shall throw a
   * runtime_error.
   */
  void write(const char* src, const std::size_t len);

  /**
   * Total number of bytes that have been written using the write method.
   */
  std::size_t totalBytesWritten() const;

 protected:
  IImageSink() = default;
  IImageSink(const IImageSink&) = delete;
  IImageSink(IImageSink&&) = default;

  /**
   * To be overwritten with the write implementation for the concrete image
   * sink.
   */
  virtual void _write(const char* const source, const std::size_t length) = 0;

 private:
  std::size_t _totalBytesWritten = 0;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
