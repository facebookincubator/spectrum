// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <string>

namespace facebook {
namespace spectrum {
namespace requirements {

/**
 * Defines how an image should be rotated.
 */
struct Rotate {
  /**
   * The angle in degrees. Note that only multiples of 90 are accepted.
   */
  int degrees{0};

  /**
   * Flips the image horizontally (on a vertical axis).
   */
  bool flipHorizontally{false};

  /**
   * Flips the image vertically (on a horizontal axis).
   */
  bool flipVertically{false};

  /**
   * Forces to rotate the bytes of the images if it contains extra orientation
   * metadata.
   */
  bool forceUpOrientation{false};

  /**
   * Whether the requirement has any effect.
   *
   * @return true if no rotation.
   */
  bool noop() const;

  /**
   * Returns the anglee between 0..<360 degrees.
   */
  int sanitisedDegrees() const;

  /**
   * Returns true if the image should be flipped on either axis.
   */
  bool flip() const;

  /**
   * String representation of a crop requirement.
   */
  std::string string() const;

  bool operator==(const Rotate& rhs) const;
  bool operator!=(const Rotate& rhs) const;
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
