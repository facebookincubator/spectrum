// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/image/Pixel.h>

#include <algorithm>
#include <vector>

namespace facebook {
namespace spectrum {
namespace image {

/**
 * A scanline represents a _horizontal_ line of the image. This wrapper allows
 * to exchange the underlying representation if needed.
 */
class Scanline {
 private:
  const pixel::Specification _specification;
  std::vector<std::uint8_t> _bytes;
  std::size_t _width;

 public:
  explicit Scanline(
      const pixel::Specification& specification,
      const std::size_t width)
      : _specification(specification),
        _bytes(specification.bytesPerPixel * width),
        _width(width){};

  /**
   * The size of the scanline in bytes in memory. It is safe to use this as the
   * limit when accessing the raw bytes using `data()`.
   */
  inline std::size_t sizeBytes() const noexcept {
    return _bytes.size();
  }

  /**
   * Returns the specification of the scanline's pixels.
   */
  const pixel::Specification specification() const noexcept {
    return _specification;
  }

  /**
   * Provides a raw pointer to the bytes in the scanline. The scanline's data is
   * guranteed to be consecutive and there are exactly `sizeBytes` available.
   */
  inline const std::uint8_t* data() const noexcept {
    return _bytes.data();
  }

  /**
   * Provides a raw pointer to the bytes in the scanline. The scanline's data is
   * guranteed to be consecutive and there are exactly `sizeBytes` available.
   */
  inline std::uint8_t* data() noexcept {
    return _bytes.data();
  }

  /**
   * Returns a raw pointer that is offsetted at the given pixel index. It is
   * safe to read `_specification.bytesPerPixel` bytes from there.
   */
  inline std::uint8_t* dataAtPixel(const std::size_t index) noexcept {
    return _bytes.data() + (index * _specification.bytesPerPixel);
  }

  /**
   * Returns a raw pointer that is offsetted at the given pixel index. It is
   * safe to read `_specification.bytesPerPixel` bytes from there.
   */
  inline const std::uint8_t* dataAtPixel(const std::size_t index) const
      noexcept {
    return _bytes.data() + (index * _specification.bytesPerPixel);
  }

  /**
   * Returns the number of pixels in the scanline.
   */
  inline std::size_t width() const noexcept {
    return _width;
  }
};

/**
 * Helper method to copy a single pixel from the input scanline to the output
 * scanline. This method does not perform range checks and the caller must
 * ensure that both indicies are within the scanlines' working areas.
 */
inline void copyPixelFromTo(
    const image::Scanline& input,
    const std::size_t inputIdx,
    image::Scanline& output,
    const std::size_t outputIdx) {
  const auto pixelSize = input.specification().bytesPerPixel;
  const auto inputOffset = pixelSize * inputIdx;
  const auto outputOffset = pixelSize * outputIdx;

  std::copy_n(
      input.data() + inputOffset, pixelSize, output.data() + outputOffset);
}

/**
 * Helper method to copy a single pixel from the input scanline to the output
 * scanline. This method does not perform range checks and the caller must
 * ensure that both indicies are within the scanlines' working areas.
 */
inline void copyPixelFromTo(
    const std::unique_ptr<image::Scanline>& input,
    const std::size_t inputIdx,
    std::unique_ptr<image::Scanline>& output,
    const std::size_t outputIdx) {
  copyPixelFromTo(*input, inputIdx, *output, outputIdx);
}

} // namespace image
} // namespace spectrum
} // namespace facebook
