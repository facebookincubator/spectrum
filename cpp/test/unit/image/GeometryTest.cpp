// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/image/Specification.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::image;

namespace facebook {
namespace spectrum {
namespace image {
namespace test {

/**
 * Ratio
 */

TEST(image_Ratio, whenIdentical_thenEqual) {
  ASSERT_EQ(Ratio({2, 1}), Ratio({2, 1}));
}

TEST(image_Ratio, whenOneComponentDifferent_thenNotEqual) {
  ASSERT_NE(Ratio({1, 1}), Ratio({2, 1}));
  ASSERT_NE(Ratio({2, 2}), Ratio({2, 1}));
}

TEST(image_Ratio, whenOne_thenOneReturnsTrue) {
  ASSERT_TRUE(Ratio({42, 42}).one());
}

TEST(image_Ratio, whenIsNotOne_thenoneReturnsFalse) {
  ASSERT_FALSE(Ratio({42, 1}).one());
}

TEST(image_Ratio, whenIsNotZero_thenZeroReturnsFalse) {
  ASSERT_FALSE(Ratio({42, 1}).zero());
}

TEST(image_Ratio, whenIsZero_thenZeroReturnsTrue) {
  ASSERT_TRUE(Ratio({0, 1}).zero());
}

/**
 * Point
 */

TEST(image_Point, whenIdentical_thenEqual) {
  ASSERT_EQ((Point{2, 1}), (Point{2, 1}));
}

TEST(image_Point, whenOneComponentDifferent_thenNotEqual) {
  ASSERT_NE((Point{2, 1}), (Point{1, 1}));
  ASSERT_NE((Point{2, 1}), (Point{2, 2}));
}

TEST(image_Point, whenRequestingStringRepresentation_thenCorrect) {
  const auto stringRepresentation = Point{42, 43}.string();
  ASSERT_EQ("{x:42,y:43}", stringRepresentation);
}

/**
 * Size
 */

TEST(image_Size, whenIdentical_thenEqual) {
  ASSERT_EQ((Size{2, 1}), (Size{2, 1}));
}

TEST(image_Size, whenOneComponentDifferent_thenNotEqual) {
  ASSERT_NE((Size{2, 1}), (Size{1, 1}));
  ASSERT_NE((Size{2, 1}), (Size{2, 2}));
}

TEST(image_Size, whenTransposed_thenSidesSwitched) {
  const auto landscape = Size{2, 1};
  const auto portrait = Size{1, 2};

  ASSERT_EQ(portrait, landscape.transposed());
}

TEST(image_Size, whenSizeEmpty_thenEmptyIsTrue) {
  ASSERT_TRUE(Size({0, 0}).empty());
  ASSERT_TRUE(Size({1, 0}).empty());
  ASSERT_TRUE(Size({0, 1}).empty());
}

TEST(image_Size, whenSizeNonNull_thenNullIsFalse) {
  ASSERT_FALSE(Size({1, 1}).empty());
}

TEST(image_Size, whenOrientated_thenSidesSwitchedInLeftRightCases) {
  const auto landscape = Size{2, 1};
  const auto portrait = Size{1, 2};

  ASSERT_EQ(landscape, landscape.oriented(Orientation::Up));
  ASSERT_EQ(landscape, landscape.oriented(Orientation::UpMirrored));
  ASSERT_EQ(landscape, landscape.oriented(Orientation::Bottom));
  ASSERT_EQ(landscape, landscape.oriented(Orientation::BottomMirrored));

  ASSERT_EQ(portrait, landscape.oriented(Orientation::LeftMirrored));
  ASSERT_EQ(portrait, landscape.oriented(Orientation::Right));
  ASSERT_EQ(portrait, landscape.oriented(Orientation::RightMirrored));
  ASSERT_EQ(portrait, landscape.oriented(Orientation::Left));
}

TEST(image_Size, whenImageSizeDefaultConstructed_thenAllFieldsZero) {
  const auto size = Size{};

  ASSERT_EQ(0, size.width);
  ASSERT_EQ(0, size.height);
}

TEST(image_Size, whenImageSizesCopied_thenFieldsAreCopied) {
  const auto size = Size{1, 2};
  const auto size2 = size;

  ASSERT_EQ(1, size2.width);
  ASSERT_EQ(2, size2.height);
}

TEST(image_Size, whenRotation90or270or450_thenWidthHeightSwapped) {
  const auto size = Size{4, 2};

  ASSERT_EQ((Size{2, 4}), size.rotated(90));
  ASSERT_EQ((Size{2, 4}), size.rotated(270));
  ASSERT_EQ((Size{2, 4}), size.rotated(450));
}

TEST(image_Size, whenRotation0Or180Or360_thenWidthHeightNotSwapped) {
  const auto size = Size{4, 2};

  ASSERT_EQ((Size{4, 2}), size.rotated(0));
  ASSERT_EQ((Size{4, 2}), size.rotated(180));
  ASSERT_EQ((Size{4, 2}), size.rotated(360));
}

TEST(image_Size, whenOrientationFlips_thenWidthHeightSwapped) {
  const auto size = Size{4, 2};

  ASSERT_EQ((Size{2, 4}), size.oriented(Orientation::Right));
  ASSERT_EQ((Size{2, 4}), size.oriented(Orientation::RightMirrored));
  ASSERT_EQ((Size{2, 4}), size.oriented(Orientation::Left));
  ASSERT_EQ((Size{2, 4}), size.oriented(Orientation::LeftMirrored));
}

TEST(image_Size, whenOrientationDoesntFlip_thenWidthHeightNotSwapped) {
  const auto size = Size{4, 2};

  ASSERT_EQ((Size{4, 2}), size.oriented(Orientation::Up));
  ASSERT_EQ((Size{4, 2}), size.oriented(Orientation::UpMirrored));
  ASSERT_EQ((Size{4, 2}), size.oriented(Orientation::Bottom));
  ASSERT_EQ((Size{4, 2}), size.oriented(Orientation::BottomMirrored));
}

TEST(image_Size, whenScaleRoundedDown_thenResultsCorrect) {
  ASSERT_EQ(
      (Size{0, 0}),
      (Size{8, 8})
          .scaled({1, 9}, core::numeric::RoundingMode::Down)); // 0 + 8/9
  ASSERT_EQ(
      (Size{1, 1}),
      (Size{8, 8})
          .scaled({2, 9}, core::numeric::RoundingMode::Down)); // 1 + 7/9
  ASSERT_EQ(
      (Size{2, 2}),
      (Size{8, 8})
          .scaled({3, 9}, core::numeric::RoundingMode::Down)); // 2 + 6/9
  ASSERT_EQ(
      (Size{3, 3}),
      (Size{8, 8})
          .scaled({4, 9}, core::numeric::RoundingMode::Down)); // 3 + 5/9
  ASSERT_EQ(
      (Size{4, 4}),
      (Size{8, 8})
          .scaled({5, 9}, core::numeric::RoundingMode::Down)); // 4 + 4/9
  ASSERT_EQ(
      (Size{5, 5}),
      (Size{8, 8})
          .scaled({6, 9}, core::numeric::RoundingMode::Down)); // 5 + 3/9
  ASSERT_EQ(
      (Size{6, 6}),
      (Size{8, 8})
          .scaled({7, 9}, core::numeric::RoundingMode::Down)); // 6 + 2/9
  ASSERT_EQ(
      (Size{7, 7}),
      (Size{8, 8})
          .scaled({8, 9}, core::numeric::RoundingMode::Down)); // 7 + 1/9
  ASSERT_EQ(
      (Size{8, 8}),
      (Size{8, 8})
          .scaled({9, 9}, core::numeric::RoundingMode::Down)); // 8 + 0/9
}

TEST(image_Size, whenScaleRoundedUp_thenResultsCorrect) {
  ASSERT_EQ(
      (Size{1, 1}),
      (Size{8, 8}).scaled({1, 9}, core::numeric::RoundingMode::Up)); // 0 + 8/9
  ASSERT_EQ(
      (Size{2, 2}),
      (Size{8, 8}).scaled({2, 9}, core::numeric::RoundingMode::Up)); // 1 + 7/9
  ASSERT_EQ(
      (Size{3, 3}),
      (Size{8, 8}).scaled({3, 9}, core::numeric::RoundingMode::Up)); // 2 + 6/9
  ASSERT_EQ(
      (Size{4, 4}),
      (Size{8, 8}).scaled({4, 9}, core::numeric::RoundingMode::Up)); // 3 + 5/9
  ASSERT_EQ(
      (Size{5, 5}),
      (Size{8, 8}).scaled({5, 9}, core::numeric::RoundingMode::Up)); // 4 + 4/9
  ASSERT_EQ(
      (Size{6, 6}),
      (Size{8, 8}).scaled({6, 9}, core::numeric::RoundingMode::Up)); // 5 + 3/9
  ASSERT_EQ(
      (Size{7, 7}),
      (Size{8, 8}).scaled({7, 9}, core::numeric::RoundingMode::Up)); // 6 + 2/9
  ASSERT_EQ(
      (Size{8, 8}),
      (Size{8, 8}).scaled({8, 9}, core::numeric::RoundingMode::Up)); // 7 + 1/9
  ASSERT_EQ(
      (Size{8, 8}),
      (Size{8, 8}).scaled({9, 9}, core::numeric::RoundingMode::Up)); // 8 + 0/9
}

TEST(image_Size, whenScaleByLargeRatio_thenResultLarger) {
  const auto size = Size{100, 100};

  ASSERT_EQ((Size{133, 133}), size.scaled({4, 3}));
}

TEST(image_Size, whenScaleOutsideUpperBounds_thenThrow) {
  const auto mid = std::numeric_limits<std::uint32_t>::max() / 2 + 1;
  const auto size = Size{mid, mid};
  ASSERT_THROW(size.scaled({2, 1}), SpectrumException);
}

TEST(image_Size, whenEqualSized_thenBothContainsAndContainedIn) {
  const auto a = Size{10, 10};
  const auto b = Size{10, 10};

  ASSERT_TRUE(a.contains(b));
  ASSERT_TRUE(a.containedIn(b));
}

TEST(image_Size, whenOneSideLarger_thenOnlyContains) {
  const auto a = Size{11, 10};
  const auto b = Size{10, 10};

  ASSERT_TRUE(a.contains(b));
  ASSERT_FALSE(a.containedIn(b));
}

TEST(image_Size, whenTwoSidesLarger_thenOnlyContains) {
  const auto a = Size{11, 11};
  const auto b = Size{10, 10};

  ASSERT_TRUE(a.contains(b));
  ASSERT_FALSE(a.containedIn(b));
}

TEST(image_Size, whenShrinkInto_thenResultsCorrect) {
  ASSERT_EQ((Size{4, 2}), (Size{5, 2}).downscaledToFit({4, 4}));
  ASSERT_EQ((Size{2, 4}), (Size{2, 5}).downscaledToFit({4, 4}));
  ASSERT_EQ((Size{200, 132}), (Size{455, 301}).downscaledToFit({200, 200}));
}

TEST(image_Size, whenRequestingStringRepresentation_thenCorrect) {
  const auto stringRepresentation = Size{42, 43}.string();
  ASSERT_EQ("{width:42,height:43}", stringRepresentation);
}

/**
 * Rect
 */

/** Coordinates */

TEST(image_Rect, whenValues_thenCoordinatesAreCorrect) {
  const auto rect = Rect{{3, 4}, {10, 12}};
  ASSERT_EQ(3, rect.minX());
  ASSERT_EQ(4, rect.minY());
  ASSERT_EQ(8, rect.midX());
  ASSERT_EQ(10, rect.midY());
  ASSERT_EQ(13, rect.maxX());
  ASSERT_EQ(16, rect.maxY());
}

/** Equality */

TEST(image_Rect, whenIdentical_thenEqual) {
  ASSERT_EQ((Rect{Point{1, 1}, Size{2, 2}}), (Rect{Point{1, 1}, Size{2, 2}}));
}

TEST(image_Rect, whenOneComponentDifferent_thenNotEqual) {
  ASSERT_NE((Rect{Point{1, 1}, Size{2, 2}}), (Rect{Point{1, 2}, Size{2, 2}}));
  ASSERT_NE((Rect{Point{1, 1}, Size{2, 2}}), (Rect{Point{1, 1}, Size{2, 1}}));
}

/** Intersection */

TEST(image_Rect, whenOneRectContainedInOther_thenIntersectionCorrect) {
  const auto largeRect = Rect{pointZero, Size{100, 100}};
  const auto smallRect = Rect{Point{25, 25}, Size{25, 25}};
  const auto expectedIntersection = Rect{Point{25, 25}, Size{25, 25}};

  ASSERT_EQ(expectedIntersection, largeRect.intersection(smallRect));
  ASSERT_EQ(expectedIntersection, smallRect.intersection(largeRect));
}

TEST(image_Rect, whenAreDisjointOnTheXAxis_thenIntersectionNull) {
  const auto rect = Rect{pointZero, Size{10, 10}};
  const auto rect2 = Rect{Point{10, 2}, Size{5, 5}};

  ASSERT_EQ(rectZero, rect.intersection(rect2));
  ASSERT_EQ(rectZero, rect2.intersection(rect));
}

TEST(image_Rect, whenAreDisjointOnTheYAxis_thenIntersectionNull) {
  const auto rect = Rect{pointZero, Size{10, 10}};
  const auto rect2 = Rect{Point{2, 10}, Size{5, 5}};

  ASSERT_EQ(rectZero, rect.intersection(rect2));
  ASSERT_EQ(rectZero, rect2.intersection(rect));
}

TEST(image_Rect, whenTwoRectIntersect_thenIntersectionReturned) {
  const auto rect = Rect{pointZero, Size{10, 20}};
  const auto rect2 = Rect{Point{4, 6}, Size{7, 2}};
  const auto expected = Rect{Point{4, 6}, Size{6, 2}};

  ASSERT_EQ(expected, rect.intersection(rect2));
  ASSERT_EQ(expected, rect2.intersection(rect));
}

TEST(image_Rect, whenTwoRectEquals_thenIntersectionIsSame) {
  const auto rect = Rect{Point{3, 4}, Size{10, 20}};

  ASSERT_EQ(rect, rect.intersection(rect));
}

TEST(image_Rect, whenRequestingStringRepresentation_thenCorrect) {
  const auto stringRepresentation = Rect{{42, 43}, {44, 45}}.string();

  ASSERT_EQ(
      "{topLeft:{x:42,y:43},size:{width:44,height:45}}", stringRepresentation);
}

} // namespace test
} // namespace image
} // namespace spectrum
} // namespace facebook
