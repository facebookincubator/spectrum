// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <cstdint>
#include <tuple>

namespace facebook {
namespace spectrum {
namespace image {

/**
 * Representation of an RGB color information.
 */
struct Color {
  std::uint8_t red;
  std::uint8_t green;
  std::uint8_t blue;

  bool operator==(const Color& rhs) const {
    return std::tie(red, green, blue) == std::tie(rhs.red, rhs.green, rhs.blue);
  }

  bool operator!=(const Color& rhs) const {
    return !(*this == rhs);
  }

  /**
   * Gray color representation as the average value of the red, green and blue
   * channels.
   */
  std::uint8_t gray() const {
    return (red + green + blue) / 3;
  }
};

} // namespace image
} // namespace spectrum
} // namespace facebook
