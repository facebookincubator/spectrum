// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <folly/Range.h>

#include <cstddef>
#include <cstdint>
#include <limits>

namespace facebook {
namespace spectrum {
namespace io {

namespace error {
extern const folly::StringPiece ImageSourceFailure;
}

/**
 * This top level interface abstracts the ability to read continious chunks of
 * data.
 *
 * `IImageSource` should not be subclassed directly. Instead, see
 * `IBitmapImageSource` and `IEncodedImageSource`.
 */
class IImageSource {
 public:
  virtual ~IImageSource() = default;

  /**
   * Read up to len bytes into the location given by *dst.
   *
   * @param destination A pointer to memory location of at least `length` bytes.
   * @param length The maximum number of bytes to read.
   * @return The number of actual bytes read. This might be smaller than the
   * value provided by length. When the end of the stream is reached 0 is
   * returned.
   */
  virtual std::size_t read(
      char* const destination,
      const std::size_t length) = 0;

  /**
   * Returns total number of bytes that have been read from the
   * underlying entity. Re-reading parts (e.g. in image sources that allow to
   * mark() and reset() the stream) will not cause this number to increase.
   */
  virtual std::size_t getTotalBytesRead() const = 0;

  /**
   * Returns a lower bound for the number of bytes that can be read from the
   * source with a subsequent read() call.
   */
  virtual std::size_t available() = 0;

 protected:
  IImageSource() = default;
  IImageSource(IImageSource&&) = default;
  IImageSource(const IImageSource&) = delete;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
