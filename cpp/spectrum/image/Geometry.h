// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/utils/Numeric.h>
#include <spectrum/image/Orientation.h>

#include <cstdint>

namespace facebook {
namespace spectrum {
namespace image {

/**
 * A ratio represents fractual numbers by a numerator and denominator.
 */
struct Ratio {
  /**
   * The ratio's numerator.
   */
  std::uint32_t numerator{1};

  /**
   * The ratio's denominator.
   */
  std::uint32_t denominator{1};

  /**
   * Returns true if the value of the represented number is 1.
   */
  bool one() const;

  /**
   * Returns true if the value of the represented number is 0.
   */
  bool zero() const;

  /**
   * Returns the value of the ratio.
   */
  double value() const;

  bool operator==(const Ratio& rhs) const;
  bool operator!=(const Ratio& rhs) const;
};

/**
 * Represents the coordinates of a point. The origin of the axis is on the top
 * left.
 */
struct Point {
  /**
   * The point's x coordinate.
   */
  std::uint32_t x{0};

  /**
   * The point's y coordinate.
   */
  std::uint32_t y{0};

  /**
   * Returns the scaled rect with the ratio rounded to the nearest values.
   *
   * @param ratio The ratio to scale the rect with.
   */
  Point scaled(const Ratio& ratio) const;

  /**
   * Returns the scaled point.
   *
   * @param ratio The ratio to scale the point with.
   * @param roundingMode The rounding mode to use.
   */
  Point scaled(
      const Ratio& ratio,
      const core::numeric::RoundingMode roundingMode) const;

  bool operator==(const Point& rhs) const;
  bool operator!=(const Point& rhs) const;

  std::string string() const;
};

constexpr auto pointZero = Point{0, 0};

/**
 * Represents a rectangular area defined by its width and height.
 *
 * When comparing two size objects A and B, A is larger than B iff both its
 * width and height are larger.
 */
struct Size {
 public:
  /**
   * The size's width.
   */
  std::uint32_t width{0};

  /**
   * The size's height.
   */
  std::uint32_t height{0};

  /**
   * Returns true if either component is 0.
   */
  bool empty() const;

  /**
   * Returns copy with width and height changed.
   */
  Size transposed() const;

  /**
   * Returns true if the point is within the size.
   */
  bool contains(const Point& other) const;

  /**
   * Returns copy width width and height after the given orientation.
   */
  Size oriented(const Orientation orientation) const;

  /**
   * Returns a rotated size with degrees.
   *
   * @param rotation in degrees of the desired angle. Method throws if called
   * with a rotation not a multiple of 90.
   */
  Size rotated(const int rotation) const;

  /**
   * Returns the product of the size with the ratio rounded to the nearest
   * value.
   *
   * @param ratio The ratio to multiply the size with.
   */
  Size scaled(const Ratio& ratio) const;

  /**
   * Returns the product of the size.
   *
   * @param ratio The ratio to multiply the size with.
   * @param roundingMode The rounding mode to use.
   */
  Size scaled(
      const Ratio& ratio,
      const core::numeric::RoundingMode roundingMode) const;

  /**
   * Returns the largest fitting size with an equal aspect ratio, rounded down.
   *
   * @param other The target size that needs to be fit into.
   */
  Size downscaledToFit(const Size& other) const;

  /**
   * Returns the largest fitting size with an equal aspect ratio.
   *
   * @param other The target size that needs to be fit into.
   * @param roundingMode The rounding mode to use.
   */
  Size downscaledToFit(
      const Size& other,
      const core::numeric::RoundingMode roundingMode) const;

  bool operator==(const Size& rhs) const;
  bool operator!=(const Size& rhs) const;

  bool operator<(const Size& rhs) const;
  bool operator<=(const Size& rhs) const;
  bool operator>=(const Size& rhs) const;
  bool operator>(const Size& rhs) const;

  std::string string() const;
};

/**
 * The empty size with both width and height equal 0.
 */
constexpr auto sizeZero = Size{0, 0};

/**
 * Defines an area and origin (top-left corner) position.
 */
struct Rect {
 public:
  /**
   * The origin of the rect, located on the top left hand size of the rect.
   */
  Point topLeft{pointZero};

  /**
   * The size of the rect.
   */
  Size size{sizeZero};

  /**
   * Returns the coordinates for the bottom-right point.
   */
  Point bottomRight() const;

  /**
   * Returns the smallest value for the x-coordinates.
   */
  std::uint32_t minX() const;

  /**
   * Returns the centre value for the x-coordinates.
   */
  std::uint32_t midX() const;

  /**
   * Returns the largest value for the x-coordinates.
   */
  std::uint32_t maxX() const;

  /**
   * Returns the smallest value for the y-coordinates.
   */
  std::uint32_t minY() const;

  /**
   * Returns the centre value for the y-coordinates.
   */
  std::uint32_t midY() const;

  /**
   * Returns the largest value for the y-coordinates.
   */
  std::uint32_t maxY() const;

  /**
   * Returns true if both rect intersect.
   *
   * @param other The rect to check for intersection.
   */
  bool intersects(const Rect& other) const;

  /**
   * Returns the intersection rect.
   *
   * @param other The rect to calculate the intersection with.
   */
  Rect intersection(const Rect& other) const;

  /**
   * Returns the scaled rect with the ratio rounded to the nearest values.
   *
   * @param ratio The ratio to scale the rect with.
   */
  Rect scaled(const Ratio& ratio) const;

  /**
   * Returns the scaled rect.
   *
   * @param ratio The ratio to scale the rect with.
   * @param roundingMode The rounding mode to use.
   */
  Rect scaled(
      const Ratio& ratio,
      const core::numeric::RoundingMode roundingMode) const;

  bool operator==(const Rect& rhs) const;
  bool operator!=(const Rect& rhs) const;

  std::string string() const;
};

/**
 * The empty rectangle situated at the origin.
 */
constexpr auto rectZero = Rect{pointZero, sizeZero};

} // namespace image
} // namespace spectrum
} // namespace facebook
