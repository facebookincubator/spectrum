// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Geometry.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/utils/Numeric.h>

#include <cmath>
#include <sstream>

namespace facebook {
namespace spectrum {
namespace image {

/**
 * Ratio
 */

bool Ratio::one() const {
  return numerator == denominator;
}

bool Ratio::zero() const {
  return numerator == 0;
}

double Ratio::value() const {
  return static_cast<double>(numerator) / denominator;
}

bool Ratio::operator==(const Ratio& rhs) const {
  return numerator == rhs.numerator && denominator == rhs.denominator;
}

bool Ratio::operator!=(const Ratio& rhs) const {
  return !(*this == rhs);
}

Point Point::scaled(const Ratio& ratio) const {
  return scaled(ratio, core::numeric::RoundingMode::Nearest);
}

Point Point::scaled(
    const Ratio& ratio,
    const core::numeric::RoundingMode roundingMode) const {
  const auto scaledX = core::numeric::rounded(x * ratio.value(), roundingMode);
  const auto scaledY = core::numeric::rounded(y * ratio.value(), roundingMode);
  return Point{
      .x = SPECTRUM_CONVERT_OR_THROW(scaledX, std::uint32_t),
      .y = SPECTRUM_CONVERT_OR_THROW(scaledY, std::uint32_t),
  };
}

/**
 * Point
 */
bool Point::operator==(const Point& rhs) const {
  return std::tie(x, y) == std::tie(rhs.x, rhs.y);
}

bool Point::operator!=(const Point& rhs) const {
  return !(*this == rhs);
}

std::string Point::string() const {
  std::stringstream ss;
  ss << "{x:" << x << ",";
  ss << "y:" << y << "}";
  return ss.str();
}

/**
 * Size
 */

bool Size::empty() const {
  return width == 0 || height == 0;
}

Size Size::transposed() const {
  return Size{
      .width = height,
      .height = width,
  };
}

Size Size::oriented(const Orientation orientation) const {
  switch (orientation) {
    case Orientation::Left:
    case Orientation::LeftMirrored:
    case Orientation::Right:
    case Orientation::RightMirrored:
      return transposed();
    case Orientation::Up:
    case Orientation::UpMirrored:
    case Orientation::Bottom:
    case Orientation::BottomMirrored:
      return *this;
    default:
      SPECTRUM_UNREACHABLE_IMAGE_ORIENTATION(orientation);
  }
}

Size Size::rotated(const int rotation) const {
  SPECTRUM_ENFORCE_IF_NOT(core::numeric::isMultipleOf90(rotation));

  if (rotation % 180 == 90) {
    return transposed();
  } else {
    return *this;
  }
}

bool Size::contains(const Point& other) const {
  return other.x <= width && other.y <= height;
}

bool Size::operator<(const Size& rhs) const {
  return width < rhs.width && height < rhs.height;
}

bool Size::operator<=(const Size& rhs) const {
  return width <= rhs.width && height <= rhs.height;
}

bool Size::operator>=(const Size& rhs) const {
  return width >= rhs.width && height >= rhs.height;
}

bool Size::operator>(const Size& rhs) const {
  return width > rhs.width && height > rhs.height;
}

bool Size::operator==(const Size& rhs) const {
  return std::tie(width, height) == std::tie(rhs.width, rhs.height);
}

bool Size::operator!=(const Size& rhs) const {
  return !(*this == rhs);
}

Size Size::scaled(const Ratio& ratio) const {
  return scaled(ratio, core::numeric::RoundingMode::Nearest);
}

Size Size::scaled(
    const Ratio& ratio,
    const core::numeric::RoundingMode roundingMode) const {
  const auto scaledWidth =
      core::numeric::rounded(width * ratio.value(), roundingMode);
  const auto scaledHeight =
      core::numeric::rounded(height * ratio.value(), roundingMode);
  return Size{.width = SPECTRUM_CONVERT_OR_THROW(scaledWidth, std::uint32_t),
              .height = SPECTRUM_CONVERT_OR_THROW(scaledHeight, std::uint32_t)};
}

Size Size::downscaledToFit(const Size& other) const {
  return downscaledToFit(other, core::numeric::RoundingMode::Nearest);
}

Size Size::downscaledToFit(
    const Size& other,
    const core::numeric::RoundingMode roundingMode) const {
  if (*this <= other) {
    return *this;
  }

  const auto scaleFactorWidth =
      static_cast<double>(width) / static_cast<double>(other.width);
  const auto scaleFactorHeight =
      static_cast<double>(height) / static_cast<double>(other.height);
  const auto scaleFactor = 1.0 / std::max(scaleFactorWidth, scaleFactorHeight);
  const auto scaledWidth =
      core::numeric::rounded(scaleFactor * width, roundingMode);
  const auto scaledHeight =
      core::numeric::rounded(scaleFactor * height, roundingMode);

  return Size{
      .width = SPECTRUM_CONVERT_OR_THROW(scaledWidth, std::uint32_t),
      .height = SPECTRUM_CONVERT_OR_THROW(scaledHeight, std::uint32_t),
  };
}

std::string Size::string() const {
  std::stringstream ss;
  ss << "{width:" << width << ",";
  ss << "height:" << height << "}";
  return ss.str();
}

/**
 * Rect
 */

bool Rect::operator==(const Rect& rhs) const {
  return std::tie(topLeft, size) == std::tie(rhs.topLeft, rhs.size);
}

bool Rect::operator!=(const Rect& rhs) const {
  return !(*this == rhs);
}

std::uint32_t Rect::minX() const {
  return topLeft.x;
}

std::uint32_t Rect::midX() const {
  return topLeft.x + size.width / 2;
}

std::uint32_t Rect::maxX() const {
  return topLeft.x + size.width;
}

std::uint32_t Rect::minY() const {
  return topLeft.y;
}

std::uint32_t Rect::midY() const {
  return topLeft.y + size.height / 2;
}

std::uint32_t Rect::maxY() const {
  return topLeft.y + size.height;
}

bool Rect::intersects(const Rect& other) const {
  const auto isOutOfBounds = maxX() <= other.minX() || maxY() <= other.minY() ||
      minX() >= other.maxX() || minY() >= other.maxY();
  return !other.size.empty() && !other.size.empty() && !isOutOfBounds;
}

Rect Rect::intersection(const Rect& other) const {
  if (intersects(other)) {
    const auto maxMinX = std::max(minX(), other.minX());
    const auto maxMinY = std::max(minY(), other.minY());
    const auto minMaxX = std::min(maxX(), other.maxX());
    const auto minMaxY = std::min(maxY(), other.maxY());
    const auto newTopLeft = Point{
        .x = std::max(topLeft.x, other.topLeft.x),
        .y = std::max(topLeft.y, other.topLeft.y),
    };
    const auto newSize = Size{
        .width = minMaxX - maxMinX,
        .height = minMaxY - maxMinY,
    };

    return Rect{newTopLeft, newSize};
  } else {
    return rectZero;
  }
}

Rect Rect::scaled(const Ratio& ratio) const {
  return scaled(ratio, core::numeric::RoundingMode::Nearest);
}

Rect Rect::scaled(
    const Ratio& ratio,
    const core::numeric::RoundingMode roundingMode) const {
  return Rect{
      .topLeft = topLeft.scaled(ratio, roundingMode),
      .size = size.scaled(ratio, roundingMode),
  };
}

Point Rect::bottomRight() const {
  return Point{
      .x = maxX(),
      .y = maxY(),
  };
}

std::string Rect::string() const {
  std::stringstream ss;
  ss << "{topLeft:" << topLeft.string() << ",";
  ss << "size:" << size.string() << "}";
  return ss.str();
}

} // namespace image
} // namespace spectrum
} // namespace facebook
