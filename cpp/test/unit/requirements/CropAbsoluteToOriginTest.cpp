// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/requirements/CropAbsoluteToOrigin.h>
#include <spectrum/core/SpectrumEnforce.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::requirements;

namespace facebook {
namespace spectrum {
namespace requirements {
namespace test {
namespace {

static constexpr auto defaultValues =
    CropAbsoluteToOrigin::Values{.left = 1, .right = 2, .top = 3, .bottom = 5};

static constexpr auto defaultImageSize = image::Size{10, 20};

void _assertValuesEquals(
    const CropAbsoluteToOrigin::Values& a,
    const CropAbsoluteToOrigin::Values& b) {
  ASSERT_EQ(a.top, b.top);
  ASSERT_EQ(a.left, b.left);
  ASSERT_EQ(a.bottom, b.bottom);
  ASSERT_EQ(a.right, b.right);
}

void _assertScaledCropRequirement(
    const folly::Optional<Crop>& cropRequirement,
    const image::Size& imageSize,
    const image::Rect& expectedCropRect) {
  ASSERT_TRUE(cropRequirement.hasValue());
  ASSERT_EQ(expectedCropRect, cropRequirement->apply(imageSize));
}

void _assertRotatedCropRequirement(
    const Rotate& rotateRequirement,
    const CropAbsoluteToOrigin::Values& expectedValues) {
  ASSERT_FALSE(rotateRequirement.flip());
  ASSERT_FALSE(rotateRequirement.forceUpOrientation);

  const auto cropRequirement = CropAbsoluteToOrigin(defaultValues, true);
  const auto rotatedCropRequirement =
      cropRequirement.rotated(rotateRequirement, defaultImageSize);
  const auto& rotatedAbsoluteCropRequirement =
      reinterpret_cast<const CropAbsoluteToOrigin&>(rotatedCropRequirement);
  const auto rotatedValues = rotatedAbsoluteCropRequirement.values();
  const auto rotatedSize =
      defaultImageSize.rotated(rotateRequirement.sanitisedDegrees());

  _assertValuesEquals(rotatedValues, expectedValues);
  ASSERT_TRUE(rotatedValues.validWithSize(rotatedSize));
}
} // namespace

TEST(
    requirements_CropAbsoluteToOrigin,
    whenLeftIsGreateOrEqualThanRight_thenThrows) {
  ASSERT_THROW(
      CropAbsoluteToOrigin(
          {.left = 1, .right = 0, .top = 0, .bottom = 1}, false),
      SpectrumException);
  ASSERT_THROW(
      CropAbsoluteToOrigin(
          {.left = 1, .right = 1, .top = 0, .bottom = 1}, false),
      SpectrumException);
}

TEST(
    requirements_CropAbsoluteToOrigin,
    whenTopIsGreaterOrEqualThanBottom_thenThrows) {
  ASSERT_THROW(
      CropAbsoluteToOrigin(
          {.left = 0, .right = 1, .top = 2, .bottom = 1}, false),
      SpectrumException);
  ASSERT_THROW(
      CropAbsoluteToOrigin(
          {.left = 0, .right = 1, .top = 1, .bottom = 1}, false),
      SpectrumException);
}

TEST(
    requirements_CropAbsoluteToOrigin,
    whenBottomIsGreaterThanImageHeight_thenThrows) {
  const auto requirement = CropAbsoluteToOrigin(
      {.left = 0, .right = 1, .top = 0, .bottom = 10}, false);

  ASSERT_THROW(requirement.apply(image::Size{10, 9}), SpectrumException);
}

TEST(
    requirements_CropAbsoluteToOrigin,
    whenRightIsGreaterThanImageWidth_thenThrows) {
  const auto requirement = CropAbsoluteToOrigin(
      {.left = 0, .right = 10, .top = 0, .bottom = 1}, false);

  ASSERT_THROW(requirement.apply(image::Size{9, 10}), SpectrumException);
}

TEST(
    requirements_CropAbsoluteToOrigin,
    whenScaledDown_thenValuesCorrectlyScaled) {
  const auto requirement = CropAbsoluteToOrigin(
      {.left = 3, .right = 6, .top = 9, .bottom = 12}, false);
  _assertScaledCropRequirement(
      requirement.scaled({1, 3}),
      image::Size{3, 5},
      image::Rect{{1, 3}, {1, 1}});
}

TEST(
    requirements_CropAbsoluteToOrigin,
    whenScaledUp_thenValuesCorrectlyScaled) {
  const auto requirement = CropAbsoluteToOrigin(
      {.left = 1, .right = 2, .top = 3, .bottom = 4}, false);
  _assertScaledCropRequirement(
      requirement.scaled({3, 1}),
      image::Size{10, 22},
      image::Rect{{3, 9}, {3, 3}});
}

TEST(
    requirements_CropAbsoluteToOrigin,
    whenScaledUpBeyondMax_thenNoneReturned) {
  const auto requirement = CropAbsoluteToOrigin(
      {.left = 0,
       .right = 20,
       .top = 30,
       .bottom = std::numeric_limits<std::uint32_t>::max() / 2 + 1},
      false);

  ASSERT_ANY_THROW(requirement.scaled({2, 1}));
}

TEST(
    requirements_CropAbsoluteToOrigin,
    whenRotatingWithUnsupportedRequirement_thenThrows) {
  ASSERT_THROW(
      CropAbsoluteToOrigin(defaultValues, false)
          .rotated(Rotate({.flipVertically = true}), defaultImageSize),
      SpectrumException);

  ASSERT_THROW(
      CropAbsoluteToOrigin(defaultValues, false)
          .rotated(Rotate({.flipHorizontally = true}), defaultImageSize),
      SpectrumException);

  ASSERT_THROW(
      CropAbsoluteToOrigin(defaultValues, false)
          .rotated(Rotate({.forceUpOrientation = true}), defaultImageSize),
      SpectrumException);

  ASSERT_THROW(
      CropAbsoluteToOrigin(defaultValues, false)
          .rotated(Rotate({.degrees = 42}), defaultImageSize),
      SpectrumException);
}

//
// Values
//

TEST(requirements_CropAbsoluteToOrigin_Values, whenRotated90_thenCorrect) {
  const auto rotateRequirement = Rotate{.degrees = 90};
  const auto expectedValues = CropAbsoluteToOrigin::Values{
      .top = 1, .left = 15, .bottom = 2, .right = 17};

  _assertRotatedCropRequirement(rotateRequirement, expectedValues);
}

TEST(requirements_CropAbsoluteToOrigin_Values, whenRotated180_thenCorrect) {
  const auto rotateRequirement = Rotate{.degrees = 180};
  const auto expectedValues = CropAbsoluteToOrigin::Values{
      .top = 15, .left = 8, .bottom = 17, .right = 9};

  _assertRotatedCropRequirement(rotateRequirement, expectedValues);
}

TEST(requirements_CropAbsoluteToOrigin_Values, whenRotated270_thenCorrect) {
  const auto rotateRequirement = Rotate{.degrees = 270};
  const auto expectedValues = CropAbsoluteToOrigin::Values{
      .top = 8, .left = 3, .bottom = 9, .right = 5};

  _assertRotatedCropRequirement(rotateRequirement, expectedValues);
}

} // namespace test
} // namespace requirements
} // namespace spectrum
} // namespace facebook
