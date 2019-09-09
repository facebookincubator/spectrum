// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/IBitmapImageSource.h>
#include <spectrum/io/IEncodedImageSource.h>

#include <cstddef>
#include <string>
#include <vector>

namespace facebook {
namespace spectrum {
namespace io {

template <class Interface>
class VectorImageSource : public Interface {
 public:
  explicit VectorImageSource(std::vector<char> data);

  VectorImageSource(const VectorImageSource&) = delete;
  VectorImageSource(VectorImageSource&&) = default;
  ~VectorImageSource() override = default;

  std::size_t read(char* const destination, const std::size_t length) override;
  std::size_t getTotalBytesRead() const override;
  std::size_t available() override;

 private:
  const std::vector<char> _data;
  std::size_t _offset = 0;
};

/**
 * The vector encoded image source wraps encoded image data.
 *
 * This is the prefered way to provide encoded image data in C++ call sites.
 */
using VectorEncodedImageSource = VectorImageSource<IEncodedImageSource>;

/**
 * The vector bitmap image source wraps decoded pixel data together with its
 * image specification.
 *
 * This is the prefered way to provide decoded image data in C++ call sites.
 */
class VectorBitmapImageSource : public VectorImageSource<IBitmapImageSource> {
 public:
  explicit VectorBitmapImageSource(
      std::vector<char> data,
      const image::Specification& imageSpecification)
      : VectorImageSource<IBitmapImageSource>(data),
        _imageSpecification(imageSpecification) {}
  image::Specification imageSpecification() const override {
    return _imageSpecification;
  }

 private:
  image::Specification _imageSpecification;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
