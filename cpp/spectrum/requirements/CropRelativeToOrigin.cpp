// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "CropRelativeToOrigin.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <sstream>

#include <cmath>

namespace facebook {
namespace spectrum {
namespace requirements {
namespace {
inline bool isRatio(const float value) {
  return value >= 0.0f && value <= 1.0f;
}

inline std::uint32_t _roundfToUint(const float value) {
  return roundf(value);
}
} // namespace

CropRelativeToOrigin::CropRelativeToOrigin(
    const Values& values,
    const bool mustBeExact)
    : Crop(std::make_unique<Handler>(values), mustBeExact) {}

CropRelativeToOrigin::Values CropRelativeToOrigin::values() const {
  return reinterpret_cast<Handler&>(*this->_handler.get()).values;
}

//
// Values
//

bool CropRelativeToOrigin::Values::valid() const {
  return isRatio(top) && isRatio(left) && isRatio(bottom) && isRatio(right) &&
      left < right && top < bottom;
}

std::string CropRelativeToOrigin::Values::string() const {
  std::stringstream ss;
  ss << "{top:" << top << ",";
  ss << "left:" << left << ",";
  ss << "bottom:" << bottom << ",";
  ss << "right:" << right << "}";
  return ss.str();
}

CropRelativeToOrigin::Values CropRelativeToOrigin::Values::rotate(
    const Rotate& rotateRequirement,
    const image::Size& size) const {
  SPECTRUM_ERROR_STRING_IF(
      rotateRequirement.flip() || rotateRequirement.forceUpOrientation,
      error::CropCannotRotateWithRequirement,
      rotateRequirement.string());

  switch (rotateRequirement.sanitisedDegrees()) {
    case 0:
      return *this;
    case 90:
      return {.top = left,
              .left = 1.0f - bottom,
              .bottom = right,
              .right = 1.0f - top};
    case 180:
      return {.top = 1.0f - bottom,
              .left = 1.0f - right,
              .bottom = 1.0f - top,
              .right = 1.0f - left};
    case 270:
      return {.top = 1.0f - right,
              .left = top,
              .bottom = 1.0f - left,
              .right = bottom};
    default:
      SPECTRUM_ERROR_STRING(
          error::CropCannotRotateWithRequirement, rotateRequirement.string());
  }
}

//
// Handler
//

CropRelativeToOrigin::Handler::Handler(
    const CropRelativeToOrigin::Values& values)
    : values(values) {
  SPECTRUM_ENFORCE_IF_NOT(values.valid());
}

image::Rect CropRelativeToOrigin::Handler::apply(
    const image::Size& size) const {
  const auto newTopLeft = image::Point{
      .x = _roundfToUint(values.left * size.width),
      .y = _roundfToUint(values.top * size.height),
  };
  const auto newBottomRight = image::Point{
      .x = _roundfToUint(values.right * size.width),
      .y = _roundfToUint(values.bottom * size.height),
  };
  const auto newSize = image::Size{
      .width = newBottomRight.x - newTopLeft.x,
      .height = newBottomRight.y - newTopLeft.y,
  };

  return {newTopLeft, newSize};
}

std::unique_ptr<Crop::IHandler> CropRelativeToOrigin::Handler::rotated(
    const Rotate& rotateRequirement,
    const image::Size& size) const {
  return std::make_unique<CropRelativeToOrigin::Handler>(
      values.rotate(rotateRequirement, size));
}

std::unique_ptr<Crop::IHandler> CropRelativeToOrigin::Handler::scaled(
    const image::Ratio& ratio) const {
  return copy();
}

std::unique_ptr<Crop::IHandler> CropRelativeToOrigin::Handler::copy() const {
  return std::make_unique<CropRelativeToOrigin::Handler>(values);
}

std::string CropRelativeToOrigin::Handler::string() const {
  return "{type:\"relativeToOrigin\",values:" + values.string() + "}";
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
