// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/requirements/CropRect.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::requirements;

namespace facebook {
namespace spectrum {
namespace requirements {
namespace test {
static constexpr auto defaultRect = image::Rect{
    .topLeft = image::Point{1, 2},
    .size = image::Size{5, 7},
};

static constexpr auto defaultImageSize = image::Size{10, 20};

namespace {

void _assertRotatedCropRequirement(
    const Rotate& rotateRequirement,
    const image::Rect& expectedRect) {
  ASSERT_FALSE(rotateRequirement.flip());
  ASSERT_FALSE(rotateRequirement.forceUpOrientation);

  const auto cropRequirement = CropRect(defaultRect, true);
  const auto rotatedCropRequirement =
      cropRequirement.rotated(rotateRequirement, defaultImageSize);
  const auto& rotatedRectCropRequirement =
      reinterpret_cast<const CropRect&>(rotatedCropRequirement);
  const auto rotatedCropRect = rotatedRectCropRequirement.rect();
  const auto rotatedImageSize =
      defaultImageSize.rotated(rotateRequirement.sanitisedDegrees());

  ASSERT_EQ(expectedRect, rotatedCropRect);
  ASSERT_TRUE(rotatedImageSize.contains(rotatedCropRect.bottomRight()));
}
} // namespace

TEST(requirements_CropRect, whenSizeIsNull_thenThrows) {
  ASSERT_THROW(CropRect(image::rectZero, false), SpectrumException);
}

TEST(requirements_CropRect, whenImageSizeIsTooSmall_thenThrows) {
  const auto requirement =
      CropRect(image::Rect{image::pointZero, image::Size{10, 20}}, false);
  ASSERT_THROW(requirement.apply({5, 100}), SpectrumException);
}

TEST(requirements_CropRect, whenRotatingWithUnsupportedRequirement_thenThrows) {
  ASSERT_THROW(
      CropRect(defaultRect, false)
          .rotated(Rotate{.flipVertically = true}, defaultImageSize),
      SpectrumException);
  ASSERT_THROW(
      CropRect(defaultRect, false)
          .rotated(Rotate{.flipHorizontally = true}, defaultImageSize),
      SpectrumException);
  ASSERT_THROW(
      CropRect(defaultRect, false)
          .rotated(Rotate{.forceUpOrientation = true}, defaultImageSize),
      SpectrumException);
  ASSERT_THROW(
      CropRect(defaultRect, false)
          .rotated(Rotate{.degrees = 42}, defaultImageSize),
      SpectrumException);
}

TEST(requirements_CropRect_Values, whenRotated90_thenCorrect) {
  const auto rotateRequirement = Rotate{.degrees = 90};
  const auto expectedRect = image::Rect{
      .topLeft = {.x = 11, .y = 1},
      .size = {.width = 7, .height = 5},
  };

  _assertRotatedCropRequirement(rotateRequirement, expectedRect);
}

TEST(requirements_CropRect_Values, whenRotated180_thenCorrect) {
  const auto rotateRequirement = Rotate{.degrees = 180};
  const auto expectedRect = image::Rect{
      .topLeft = {.x = 4, .y = 11},
      .size = {.width = 5, .height = 7},
  };

  _assertRotatedCropRequirement(rotateRequirement, expectedRect);
}

TEST(requirements_CropRect_Values, whenRotated270_thenCorrect) {
  const auto rotateRequirement = Rotate{.degrees = 270};
  const auto expectedRect = image::Rect{
      .topLeft = {.x = 2, .y = 4},
      .size = {.width = 7, .height = 5},
  };

  _assertRotatedCropRequirement(rotateRequirement, expectedRect);
}

} // namespace test
} // namespace requirements
} // namespace spectrum
} // namespace facebook
