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

class VectorBitmapImageSource;
class VectorEncodedImageSource;

class VectorImageSourceHandler {
  friend class VectorBitmapImageSource;
  friend class VectorEncodedImageSource;

 private:
  explicit VectorImageSourceHandler(std::vector<char> data);
  std::size_t _read(char* const destination, const std::size_t length);
  std::size_t _getTotalBytesRead() const;
  std::size_t _available();

  const std::vector<char> _data;
  std::size_t _offset = 0;
};

/**
 * The vector bitmap image source wraps decoded pixel data together with its
 * image specification.
 *
 * This is the prefered way to provide decoded image data in C++ call sites.
 */
class VectorBitmapImageSource : public IBitmapImageSource {
 public:
  /**
   * New vector bitmap image source with the given pixel data and specification.
   * The match of the vector and image specification is not enforced during
   * construction.
   *
   * @param data vector of pixel data that will be consumed by `read` calls.
   * @param imageSpecification image specification that matches the provided
   * pixel data.
   */
  explicit VectorBitmapImageSource(
      std::vector<char> data,
      const image::Specification& imageSpecification);

  VectorBitmapImageSource(const VectorBitmapImageSource&) = delete;
  VectorBitmapImageSource(VectorBitmapImageSource&&) = default;
  ~VectorBitmapImageSource() override = default;

  std::size_t read(char* const destination, const std::size_t length) override;
  std::size_t getTotalBytesRead() const override;
  std::size_t available() override;
  image::Specification imageSpecification() const override;

 private:
  VectorImageSourceHandler _delegate;
  image::Specification _imageSpecification;
};

/**
 * The vector bitmap image source wraps encoded image data.
 *
 * This is the prefered way to provide encoded image data in C++ call sites.
 */
class VectorEncodedImageSource : public IEncodedImageSource {
 public:
  /**
   * @param data vector with the encoded image bytes
   */
  explicit VectorEncodedImageSource(std::vector<char> data);

  VectorEncodedImageSource(const VectorEncodedImageSource&) = delete;
  VectorEncodedImageSource(VectorEncodedImageSource&&) = default;
  ~VectorEncodedImageSource() override = default;

  std::size_t read(char* const destination, const std::size_t length) override;
  std::size_t getTotalBytesRead() const override;
  std::size_t available() override;

 private:
  VectorImageSourceHandler _delegate;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
