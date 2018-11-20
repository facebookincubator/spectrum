// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "CropRect.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/utils/Numeric.h>

#include <sstream>

namespace facebook {
namespace spectrum {
namespace requirements {
namespace {
image::Point _rotatedTopLeft(
    const image::Rect& rect,
    const image::Size& imageSize,
    const requirements::Rotate& rotateRequirement) {
  switch (rotateRequirement.sanitisedDegrees()) {
    case 90:
      return image::Point{
          .x = imageSize.height - rect.topLeft.y - rect.size.height,
          .y = rect.topLeft.x,
      };
    case 180:
      return image::Point{
          .x = imageSize.width - rect.topLeft.x - rect.size.width,
          .y = imageSize.height - rect.topLeft.y - rect.size.height,
      };
    case 270:
      return image::Point{
          .x = rect.topLeft.y,
          .y = imageSize.width - rect.topLeft.x - rect.size.width,
      };
    default:
      SPECTRUM_ERROR_STRING(
          error::CropCannotRotateWithRequirement, rotateRequirement.string());
  }
}
} // namespace

CropRect::CropRect(const image::Rect& rect, const bool mustBeExact)
    : Crop(std::make_unique<Handler>(rect), mustBeExact) {}

image::Rect CropRect::rect() const {
  return reinterpret_cast<Handler&>(*this->_handler.get()).rect;
}

/**
 * Handler
 */

CropRect::Handler::Handler(const image::Rect& rect) : rect(rect) {
  SPECTRUM_ENFORCE_IF(rect.size.empty());
}

image::Rect CropRect::Handler::apply(const image::Size& size) const {
  SPECTRUM_ENFORCE_IF(rect.maxX() > size.width || rect.maxY() > size.height);
  return rect;
}

std::unique_ptr<Crop::IHandler> CropRect::Handler::rotated(
    const Rotate& rotateRequirement,
    const image::Size& size) const {
  return std::make_unique<Handler>(image::Rect{
      .topLeft = _rotatedTopLeft(rect, size, rotateRequirement),
      .size = rect.size.rotated(rotateRequirement.sanitisedDegrees()),
  });
}

std::unique_ptr<Crop::IHandler> CropRect::Handler::scaled(
    const image::Ratio& ratio) const {
  return std::make_unique<Handler>(rect.scaled(ratio));
}

std::unique_ptr<Crop::IHandler> CropRect::Handler::copy() const {
  return std::make_unique<Handler>(rect);
}

std::string CropRect::Handler::string() const {
  return "{type:\"rect\",rect: " + rect.string() + "}";
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
