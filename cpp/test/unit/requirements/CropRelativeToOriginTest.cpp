// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/requirements/CropRelativeToOrigin.h>
#include <spectrum/core/SpectrumEnforce.h>

#include <gtest/gtest.h>

#include <cmath>

using namespace facebook::spectrum::requirements;

namespace facebook {
namespace spectrum {
namespace requirements {
namespace test {
namespace {

static constexpr auto defaultValues =
    CropRelativeToOrigin::Values{.left = 0.1f,
                                 .right = 0.8f,
                                 .top = 0.5f,
                                 .bottom = 0.75f};

static constexpr auto defaultImageSize = image::Size{10, 20};

void assertValuesEquals(
    const CropRelativeToOrigin::Values& a,
    const CropRelativeToOrigin::Values& b) {
  const auto floatEpsilon = std::numeric_limits<float>::epsilon();
  ASSERT_TRUE(std::fabs(a.top - b.top) < floatEpsilon);
  ASSERT_TRUE(std::fabs(a.left - b.left) < floatEpsilon);
  ASSERT_TRUE(std::fabs(a.bottom - b.bottom) < floatEpsilon);
  ASSERT_TRUE(std::fabs(a.right - b.right) < floatEpsilon);
}

void assertRotatedCropRequirement(
    const Rotate& rotateRequirement,
    const CropRelativeToOrigin::Values& expectedValues) {
  ASSERT_FALSE(rotateRequirement.flip());
  ASSERT_FALSE(rotateRequirement.forceUpOrientation);

  const auto cropRequirement = CropRelativeToOrigin(defaultValues, true);
  const auto rotatedCropRequirement =
      cropRequirement.rotated(rotateRequirement, defaultImageSize);
  const auto& rotatedRelativeCropRequirement =
      reinterpret_cast<const CropRelativeToOrigin&>(rotatedCropRequirement);
  const auto rotatedValues = rotatedRelativeCropRequirement.values();

  assertValuesEquals(rotatedValues, expectedValues);
  ASSERT_TRUE(rotatedValues.valid());
}
} // namespace

TEST(
    requirements_CropRelativeToOrigin,
    whenLeftIsGreateOrEqualThanRight_thenThrows) {
  ASSERT_THROW(
      CropRelativeToOrigin(
          {.left = 0.2f, .right = 0.1f, .top = 0.0f, .bottom = 1.0f}, false),
      SpectrumException);
  ASSERT_THROW(
      CropRelativeToOrigin(
          {.left = 0.5f, .right = 0.5f, .top = 0.0f, .bottom = 1.0f}, false),
      SpectrumException);
}

TEST(
    requirements_CropRelativeToOrigin,
    whenTopIsGreaterOrEqualThanBottom_thenThrows) {
  ASSERT_THROW(
      CropRelativeToOrigin(
          {.left = 0.0f, .right = 1.0f, .top = 0.2f, .bottom = 0.1f}, false),
      SpectrumException);
  ASSERT_THROW(
      CropRelativeToOrigin(
          {.left = 0.0f, .right = 1.0f, .top = 0.5f, .bottom = 0.5f}, false),
      SpectrumException);
}

TEST(requirements_CropRelativeToOrigin, whenValuesSmallerThanZero_thenThrows) {
  ASSERT_THROW(
      CropRelativeToOrigin(
          {.left = -0.1f, .right = 1.0f, .top = 0.0f, .bottom = 1.0f}, false),
      SpectrumException);
  ASSERT_THROW(
      CropRelativeToOrigin(
          {.left = 0.0f, .right = 1.0f, .top = -0.1f, .bottom = 1.0f}, false),
      SpectrumException);
}

TEST(requirements_CropRelativeToOrigin, whenValuesGreaterThanOne_thenThrows) {
  ASSERT_THROW(
      CropRelativeToOrigin(
          {.left = 0.0f, .right = 1.1f, .top = 0.0f, .bottom = 1.0f}, false),
      SpectrumException);
  ASSERT_THROW(
      CropRelativeToOrigin(
          {.left = 0.0f, .right = 1.0f, .top = 0.0f, .bottom = 1.1f}, false),
      SpectrumException);
}

TEST(
    requirements_CropRelativeToOrigin,
    whenRotatingWithUnsupportedRequirement_thenThrows) {
  ASSERT_THROW(
      CropRelativeToOrigin(defaultValues, false)
          .rotated(Rotate({.flipVertically = true}), defaultImageSize),
      SpectrumException);

  ASSERT_THROW(
      CropRelativeToOrigin(defaultValues, false)
          .rotated(Rotate({.flipHorizontally = true}), defaultImageSize),
      SpectrumException);

  ASSERT_THROW(
      CropRelativeToOrigin(defaultValues, false)
          .rotated(Rotate({.forceUpOrientation = true}), defaultImageSize),
      SpectrumException);

  ASSERT_THROW(
      CropRelativeToOrigin(defaultValues, false)
          .rotated(Rotate({.degrees = 42}), defaultImageSize),
      SpectrumException);
}

//
// Values
//

TEST(requirements_CropRelativeToOrigin_Values, whenRotated90_thenCorrect) {
  const auto rotateRequirement = Rotate{.degrees = 90};
  const auto expectedValues = CropRelativeToOrigin::Values{
      .top = 0.1f, .left = 0.25f, .bottom = 0.8f, .right = 0.5f};

  assertRotatedCropRequirement(rotateRequirement, expectedValues);
}

TEST(requirements_CropRelativeToOrigin_Values, whenRotated180_thenCorrect) {
  const auto rotateRequirement = Rotate{.degrees = 180};
  const auto expectedValues = CropRelativeToOrigin::Values{
      .top = 0.25f, .left = 0.2f, .bottom = 0.5f, .right = 0.9f};

  assertRotatedCropRequirement(rotateRequirement, expectedValues);
}

TEST(requirements_CropRelativeToOrigin_Values, whenRotated270_thenCorrect) {
  const auto rotateRequirement = Rotate{.degrees = 270};
  const auto expectedValues = CropRelativeToOrigin::Values{
      .top = 0.2f, .left = 0.5f, .bottom = 0.9f, .right = 0.75f};

  assertRotatedCropRequirement(rotateRequirement, expectedValues);
}

} // namespace test
} // namespace test
} // namespace spectrum
} // namespace facebook
