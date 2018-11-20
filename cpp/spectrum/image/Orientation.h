// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <folly/Optional.h>

#include <cstdint>
#include <string>

namespace facebook {
namespace spectrum {
namespace image {

/**
 * The orientation describes the transformation that needs to be applied to the
 * underlying image in order to reach its intended representation.
 */
enum class Orientation : std::uint16_t {
  Up = 1,
  UpMirrored = 2,
  Bottom = 3,
  BottomMirrored = 4,
  LeftMirrored = 5,
  Right = 6,
  RightMirrored = 7,
  Left = 8,
};

#define SPECTRUM_UNREACHABLE_IMAGE_ORIENTATION(orientation) \
  SPECTRUM_UNREACHABLE_STRING(                              \
      folly::StringPiece{"unreachable_image_orientation"},  \
      orientationStringFromValue(orientation))

/** The default orientation (Up) */
extern const Orientation orientationDefault;

/**
 * Creates an orientation from its value. Throws if the value is invalid.
 *
 * @param value The value to create the orientation from.
 */
Orientation orientationFromValue(const std::uint16_t value);

/**
 * Rotates an orientation.
 *
 * @param orientation The orientation to rotate.
 * @param degrees The rotation to apply. Needs to be a multiple of 90.
 */
Orientation orientationRotatedWithDegrees(
    const Orientation orientation,
    const int degrees);

/**
 * Rotates an orientation with both a horizontal and a vertical flip.
 *
 * @param orientation The orientation to rotate.
 * @param degrees The rotation to apply. Needs to be a multiple of 90.
 * @param flipHorizontally if the orientation should be flipped horizontally
 * after being rotated.
 * @param flipVertically if the orientation should be flipped vertically after
 * being rotated.
 */
Orientation orientationRotatedAndFlipped(
    const Orientation orientation,
    const int degrees,
    const bool flipHorizontally,
    const bool flipVertically);

/**
 * Horizotally flips the orientation.
 *
 * @param orientation The orientation to flip.
 */
Orientation orientationFlippedHorizontally(const Orientation orientation);

/**
 * Vectically flips the orientation.
 *
 * @param orientation The orientation to flip.
 */
Orientation orientationFlippedVertically(const Orientation orientation);

/**
 * The orientation's string representation.
 *
 * @param orientation The orientation to convert.
 */
std::string orientationStringFromValue(const Orientation orientation);

} // namespace image
} // namespace spectrum
} // namespace facebook
