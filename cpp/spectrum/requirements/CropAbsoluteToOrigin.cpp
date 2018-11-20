// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "CropAbsoluteToOrigin.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <sstream>

namespace facebook {
namespace spectrum {
namespace requirements {

CropAbsoluteToOrigin::CropAbsoluteToOrigin(
    const Values& values,
    const bool mustBeExact)
    : Crop(std::make_unique<Handler>(values), mustBeExact) {}

bool CropAbsoluteToOrigin::Values::valid() const {
  return left < right && top < bottom;
}

CropAbsoluteToOrigin::Values CropAbsoluteToOrigin::values() const {
  return reinterpret_cast<Handler&>(*this->_handler.get()).values;
}

//
// Values
//

std::string CropAbsoluteToOrigin::Values::string() const {
  std::stringstream ss;
  ss << "{top:" << top << ",";
  ss << "left:" << left << ",";
  ss << "bottom:" << bottom << ",";
  ss << "right:" << right << "}";
  return ss.str();
}

bool CropAbsoluteToOrigin::Values::validWithSize(
    const image::Size& size) const {
  return valid() && right <= size.width && bottom <= size.height;
}

CropAbsoluteToOrigin::Values CropAbsoluteToOrigin::Values::rotate(
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
              .left = size.height - bottom,
              .bottom = right,
              .right = size.height - top};
    case 180:
      return {.top = size.height - bottom,
              .left = size.width - right,
              .bottom = size.height - top,
              .right = size.width - left};
    case 270:
      return {.top = size.width - right,
              .left = top,
              .bottom = size.width - left,
              .right = bottom};
    default:
      SPECTRUM_ERROR_STRING(
          error::CropCannotRotateWithRequirement, rotateRequirement.string());
  }
}

//
// Handler
//

CropAbsoluteToOrigin::Handler::Handler(const Values& values) : values(values) {
  SPECTRUM_ENFORCE_IF_NOT(values.valid());
}

image::Rect CropAbsoluteToOrigin::Handler::apply(
    const image::Size& size) const {
  SPECTRUM_ENFORCE_IF_NOT(values.validWithSize(size));
  return {.topLeft =
              image::Point{
                  .x = values.left,
                  .y = values.top,
              },
          .size = image::Size{
              .width = values.right - values.left,
              .height = values.bottom - values.top,
          }};
}

std::unique_ptr<Crop::IHandler> CropAbsoluteToOrigin::Handler::rotated(
    const Rotate& rotateRequirement,
    const image::Size& size) const {
  return std::make_unique<Handler>(values.rotate(rotateRequirement, size));
}

std::unique_ptr<Crop::IHandler> CropAbsoluteToOrigin::Handler::scaled(
    const image::Ratio& ratio) const {
  const auto top =
      SPECTRUM_CONVERT_OR_THROW(values.top * ratio.value(), std::uint32_t);
  const auto left =
      SPECTRUM_CONVERT_OR_THROW(values.left * ratio.value(), std::uint32_t);
  const auto bottom =
      SPECTRUM_CONVERT_OR_THROW(values.bottom * ratio.value(), std::uint32_t);
  const auto right =
      SPECTRUM_CONVERT_OR_THROW(values.right * ratio.value(), std::uint32_t);
  return std::make_unique<Handler>(
      Values{.top = top, .left = left, .bottom = bottom, .right = right});
}

std::unique_ptr<Crop::IHandler> CropAbsoluteToOrigin::Handler::copy() const {
  return std::make_unique<Handler>(values);
}

std::string CropAbsoluteToOrigin::Handler::string() const {
  return "{type:\"absoluteToOrigin\",values:" + values.string() + "}";
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
