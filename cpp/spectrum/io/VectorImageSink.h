// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/Constants.h>
#include <spectrum/io/IBitmapImageSink.h>

#include <folly/Optional.h>

#include <cstddef>
#include <string>
#include <vector>

namespace facebook {
namespace spectrum {
namespace io {

/**
 * The vector image sink writes bitmap data to a continious buffer in memory
 * which is exposed as a std::vector reference. The size and pixel specification
 * of the bitmap is exposed as well.
 *
 * This is the prefered way to retrieve decoded image data in C++ call sites.
 */
class VectorImageSink : public IBitmapImageSink {
 private:
  std::vector<char> _data;
  folly::Optional<image::Size> _imageSize;
  folly::Optional<image::pixel::Specification> _pixelSpecification;

 public:
  explicit VectorImageSink(
      const std::size_t initialCapacity = core::DefaultBufferSize);
  VectorImageSink(const VectorImageSink&) = delete;
  VectorImageSink(VectorImageSink&&) = default;

  ~VectorImageSink() override = default;

  void setConfiguration(
      const image::Size& imageSize,
      const image::pixel::Specification& pixelSpecification) override;

  /**
   * Returns a reference to the written pixel data.
   */
  std::vector<char>& getVectorReference() {
    return _data;
  }

  /**
   * The size of the written bitmap.
   */
  folly::Optional<image::Size> size() const {
    return _imageSize;
  }

  /**
   * The pixel specification of the written bitmap.
   */
  folly::Optional<image::pixel::Specification> getPixelSpecification() const {
    return _pixelSpecification;
  }

 protected:
  void _write(const char* const source, const std::size_t length) override;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
