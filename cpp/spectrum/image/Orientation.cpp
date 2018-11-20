// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Orientation.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/utils/Numeric.h>

namespace facebook {
namespace spectrum {
namespace image {

const Orientation orientationDefault = Orientation::Up;

Orientation orientationFromValue(const std::uint16_t value) {
  const auto orientation = static_cast<Orientation>(value);

  if (value < static_cast<std::uint16_t>(Orientation::Up) ||
      value > static_cast<std::uint16_t>(Orientation::Left)) {
    SPECTRUM_UNREACHABLE_IMAGE_ORIENTATION(orientation);
  } else {
    return orientation;
  }
}

static Orientation orientationRotated90DegreesClockwise(
    const Orientation orientation) {
  switch (orientation) {
    case Orientation::Up:
      return Orientation::Right;
    case Orientation::Right:
      return Orientation::Bottom;
    case Orientation::Bottom:
      return Orientation::Left;
    case Orientation::Left:
      return Orientation::Up;
    case Orientation::UpMirrored:
      return Orientation::RightMirrored;
    case Orientation::RightMirrored:
      return Orientation::BottomMirrored;
    case Orientation::BottomMirrored:
      return Orientation::LeftMirrored;
    case Orientation::LeftMirrored:
      return Orientation::UpMirrored;
    default:
      SPECTRUM_UNREACHABLE_IMAGE_ORIENTATION(orientation);
  }
}

Orientation orientationRotatedWithDegrees(
    const Orientation orientation,
    const int degrees) {
  SPECTRUM_ENFORCE_IF_NOT(core::numeric::isMultipleOf90(degrees));

  auto currentDegrees = core::numeric::angleDegreesSanitized(degrees);
  auto result = orientation;
  for (; currentDegrees > 0; currentDegrees -= 90) {
    result = orientationRotated90DegreesClockwise(result);
  }

  return result;
}

Orientation orientationRotatedAndFlipped(
    const Orientation orientation,
    const int degrees,
    const bool flipHorizontally,
    const bool flipVertically) {
  auto result = orientationRotatedWithDegrees(orientation, degrees);

  if (flipHorizontally) {
    result = orientationFlippedHorizontally(result);
  }

  if (flipVertically) {
    result = orientationFlippedVertically(result);
  }

  return result;
}

Orientation orientationFlippedHorizontally(const Orientation orientation) {
  switch (orientation) {
    case Orientation::Up:
      return Orientation::UpMirrored;
    case Orientation::Right:
      return Orientation::RightMirrored;
    case Orientation::Bottom:
      return Orientation::BottomMirrored;
    case Orientation::Left:
      return Orientation::LeftMirrored;
    case Orientation::UpMirrored:
      return Orientation::Up;
    case Orientation::RightMirrored:
      return Orientation::Right;
    case Orientation::BottomMirrored:
      return Orientation::Bottom;
    case Orientation::LeftMirrored:
      return Orientation::Left;
    default:
      SPECTRUM_UNREACHABLE_IMAGE_ORIENTATION(orientation);
  }
}

Orientation orientationFlippedVertically(const Orientation orientation) {
  switch (orientation) {
    case Orientation::Up:
      return Orientation::BottomMirrored;
    case Orientation::Right:
      return Orientation::LeftMirrored;
    case Orientation::Bottom:
      return Orientation::UpMirrored;
    case Orientation::Left:
      return Orientation::RightMirrored;
    case Orientation::UpMirrored:
      return Orientation::Bottom;
    case Orientation::RightMirrored:
      return Orientation::Left;
    case Orientation::BottomMirrored:
      return Orientation::Up;
    case Orientation::LeftMirrored:
      return Orientation::Right;
    default:
      SPECTRUM_UNREACHABLE_IMAGE_ORIENTATION(orientation);
  }
}

std::string orientationStringFromValue(const Orientation orientation) {
  switch (orientation) {
    case Orientation::Up:
      return "up";
    case Orientation::Right:
      return "right";
    case Orientation::Bottom:
      return "bottom";
    case Orientation::Left:
      return "left";
    case Orientation::UpMirrored:
      return "up_mirrored";
    case Orientation::RightMirrored:
      return "right_mirrored";
    case Orientation::BottomMirrored:
      return "bottom_mirrored";
    case Orientation::LeftMirrored:
      return "left_mirrored";
    default:
      return core::makeUnknownWithValue<std::uint16_t>(orientation);
  }
}

} // namespace image
} // namespace spectrum
} // namespace facebook
