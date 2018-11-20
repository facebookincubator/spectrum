// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/IEncodedImageSource.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace facebook {
namespace spectrum {
namespace io {

/**
 * RewindableImageSource wraps the underlying image source with a buffer to
 * allow reading buffered portions multiple times.
 *
 * When in the "marked" state, it allows to reset subsequent read-head
 * advancements. After the reset the bytes that have been read in between "mark"
 * and "reset" will be read again.
 */
class RewindableImageSource : public IEncodedImageSource {
 private:
  IImageSource& imageSource;
  std::vector<char> buffer;

  /**
   * The offset is '0' exactly when we are reading the next byte from the
   * original input stream. It is larger than 0, if the read head is within the
   * buffer.
   *
   * E.g.: If the offset is 3 and the buffer has 10 elements, we would read the
   * element buffer[10 - 3 - 1] (i.e. the 7th element);
   */
  std::size_t offset = 0;

  /**
   * When true, all subsequent reads will fill the buffer and offset must be 0.
   */
  bool isMarkActive = false;

 public:
  /**
   * Creates a RewindableImageSource which wraps the
   */
  explicit RewindableImageSource(IImageSource& imageSource);
  RewindableImageSource(const RewindableImageSource&) = delete;
  RewindableImageSource(RewindableImageSource&&) = default;

  ~RewindableImageSource() override = default;

  std::size_t read(char* const destination, const std::size_t length) override;

  std::size_t getTotalBytesRead() const override {
    return imageSource.getTotalBytesRead();
  }

  std::size_t available() override;

  /**
   * Marks the current position as the start of the buffer. A call to reset()
   * will be able to logically reset the read head to this position in order
   * to re-read the content.
   */
  void mark();

  /**
   * Will logically reset the read head to the position when mark() was called.
   * Everything that has been read between that call of mark() and now, will be
   * read a second time.
   *
   * If reset() is called without a matching mark() (or there has been a call to
   * reset() in between), a runtime_error will be thrown.
   *
   * If you want to re-read the same sequence multiple times, use the following
   * pattern:
   *
   *   source.mark();
   *   ... = source.read(...); // read 1st time
   *   source.reset();
   *   source.mark();
   *   ... = source.read(...); // read 2nd time
   *   source.reset();
   *   ... = source.read(...); // read 3rd and last time
   */
  void reset();
};

} // namespace io
} // namespace spectrum
} // namespace facebook
