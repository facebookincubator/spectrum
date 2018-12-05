// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/Orientation.h>
#include <spectrum/core/SpectrumEnforce.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::image;

namespace facebook {
namespace spectrum {
namespace image {
namespace test {

TEST(image_image_Orientation, whenCreatedFromWrongValue_thenThrows) {
  ASSERT_THROW(orientationFromValue(9), SpectrumException);
  ASSERT_THROW(orientationFromValue(0), SpectrumException);
}

TEST(image_image_Orientation, whenCreatedFromValidValue_thenReturned) {
  ASSERT_EQ(Orientation::Up, orientationFromValue(1));
  ASSERT_EQ(Orientation::Left, orientationFromValue(8));
}

TEST(
    image_Orientation,
    whenCreatingWithAnAngleNotBetween0And360_thenItIsSanitized) {
  ASSERT_EQ(
      Orientation::Bottom,
      orientationRotatedWithDegrees(Orientation::Bottom, -360));
  ASSERT_EQ(
      Orientation::Up, orientationRotatedWithDegrees(Orientation::Bottom, 540));
  ASSERT_EQ(
      Orientation::Left, orientationRotatedWithDegrees(Orientation::Up, 630));
  ASSERT_EQ(
      Orientation::Right,
      orientationRotatedWithDegrees(Orientation::Bottom, -90));
}

TEST(image_Orientation, whenCreatingWithValuesMatchingUp_thenUpIsReturned) {
  ASSERT_EQ(Orientation::Up, orientationRotatedWithDegrees(Orientation::Up, 0));
  ASSERT_EQ(
      Orientation::Up, orientationRotatedWithDegrees(Orientation::Left, 90));
  ASSERT_EQ(
      Orientation::Up, orientationRotatedWithDegrees(Orientation::Right, -90));
}

TEST(
    image_Orientation,
    whenCreatingWithValuesMatchingRight_thenRightIsReturned) {
  ASSERT_EQ(
      Orientation::Right, orientationRotatedWithDegrees(Orientation::Up, 90));
  ASSERT_EQ(
      Orientation::Right, orientationRotatedWithDegrees(Orientation::Right, 0));
  ASSERT_EQ(
      Orientation::Right, orientationRotatedWithDegrees(Orientation::Up, 90));
  ASSERT_EQ(
      Orientation::Right,
      orientationRotatedWithDegrees(Orientation::Bottom, -90));
}

TEST(
    image_Orientation,
    whenCreatingWithValuesMatchingBottom_thenBottomIsReturned) {
  ASSERT_EQ(
      Orientation::Bottom, orientationRotatedWithDegrees(Orientation::Up, 180));
  ASSERT_EQ(
      Orientation::Bottom,
      orientationRotatedWithDegrees(Orientation::Bottom, 0));
  ASSERT_EQ(
      Orientation::Bottom,
      orientationRotatedWithDegrees(Orientation::Right, 90));
  ASSERT_EQ(
      Orientation::Bottom,
      orientationRotatedWithDegrees(Orientation::Left, -90));
}

TEST(image_Orientation, whenCreatingWithValuesMatchingLeft_thenLeftIsReturned) {
  ASSERT_EQ(
      Orientation::Left, orientationRotatedWithDegrees(Orientation::Up, 270));
  ASSERT_EQ(
      Orientation::Left, orientationRotatedWithDegrees(Orientation::Left, 0));
  ASSERT_EQ(
      Orientation::Left,
      orientationRotatedWithDegrees(Orientation::Bottom, 90));
  ASSERT_EQ(
      Orientation::Left, orientationRotatedWithDegrees(Orientation::Up, -90));
}

TEST(
    image_Orientation,
    whenCreatingWithValuesMatchingUpMirrored_thenUpMirroredIsReturned) {
  ASSERT_EQ(
      Orientation::UpMirrored,
      orientationRotatedWithDegrees(Orientation::UpMirrored, 0));
  ASSERT_EQ(
      Orientation::UpMirrored,
      orientationRotatedWithDegrees(Orientation::LeftMirrored, 90));
  ASSERT_EQ(
      Orientation::UpMirrored,
      orientationRotatedWithDegrees(Orientation::RightMirrored, -90));
}

TEST(
    image_Orientation,
    whenCreatingWithValuesMatchingRightMirrored_thenRightMirroredIsReturned) {
  ASSERT_EQ(
      Orientation::RightMirrored,
      orientationRotatedWithDegrees(Orientation::RightMirrored, 0));
  ASSERT_EQ(
      Orientation::RightMirrored,
      orientationRotatedWithDegrees(Orientation::UpMirrored, 90));
  ASSERT_EQ(
      Orientation::RightMirrored,
      orientationRotatedWithDegrees(Orientation::BottomMirrored, -90));
}

TEST(
    image_Orientation,
    whenCreatingWithValuesMatchingBottomMirrored_thenBottomMirroredIsReturned) {
  ASSERT_EQ(
      Orientation::BottomMirrored,
      orientationRotatedWithDegrees(Orientation::BottomMirrored, 0));
  ASSERT_EQ(
      Orientation::BottomMirrored,
      orientationRotatedWithDegrees(Orientation::RightMirrored, 90));
  ASSERT_EQ(
      Orientation::BottomMirrored,
      orientationRotatedWithDegrees(Orientation::LeftMirrored, -90));
}

TEST(
    image_Orientation,
    whenCreatingWithValuesMatchingLeftMirrored_thenLeftMirroredIsReturned) {
  ASSERT_EQ(
      Orientation::LeftMirrored,
      orientationRotatedWithDegrees(Orientation::LeftMirrored, 0));
  ASSERT_EQ(
      Orientation::LeftMirrored,
      orientationRotatedWithDegrees(Orientation::BottomMirrored, 90));
  ASSERT_EQ(
      Orientation::LeftMirrored,
      orientationRotatedWithDegrees(Orientation::UpMirrored, -90));
}

TEST(
    image_Orientation,
    whenMergeFunctionNotMirroring_thenSameEffectAsJustRotationFunction) {
  ASSERT_EQ(
      orientationRotatedWithDegrees(Orientation::BottomMirrored, 90),
      orientationRotatedAndFlipped(
          Orientation::BottomMirrored, 90, false, false));
  ASSERT_EQ(
      orientationRotatedWithDegrees(Orientation::Up, 0),
      orientationRotatedAndFlipped(Orientation::Up, 0, false, false));
}

TEST(image_Orientation, whenMergeFunctionMirroring_thenUpResultsInUpMirrored) {
  ASSERT_EQ(
      Orientation::UpMirrored,
      orientationRotatedAndFlipped(Orientation::Up, 0, true, false));
}

TEST(image_Orientation, whenMergeFunctionRotatingAndMirroring_thenCorrect) {
  ASSERT_EQ(
      Orientation::LeftMirrored,
      orientationRotatedAndFlipped(Orientation::Up, 270, true, false));
}

TEST(image_Orientation, whenMirrored_thenMirroredValueReturned) {
  // non-mirror to mirrored
  ASSERT_EQ(
      Orientation::UpMirrored, orientationFlippedHorizontally(Orientation::Up));
  ASSERT_EQ(
      Orientation::RightMirrored,
      orientationFlippedHorizontally(Orientation::Right));
  ASSERT_EQ(
      Orientation::BottomMirrored,
      orientationFlippedHorizontally(Orientation::Bottom));
  ASSERT_EQ(
      Orientation::LeftMirrored,
      orientationFlippedHorizontally(Orientation::Left));

  // mirrored to non-mirrored
  ASSERT_EQ(
      Orientation::Up, orientationFlippedHorizontally(Orientation::UpMirrored));
  ASSERT_EQ(
      Orientation::Right,
      orientationFlippedHorizontally(Orientation::RightMirrored));
  ASSERT_EQ(
      Orientation::Bottom,
      orientationFlippedHorizontally(Orientation::BottomMirrored));
  ASSERT_EQ(
      Orientation::Left,
      orientationFlippedHorizontally(Orientation::LeftMirrored));
}

TEST(image_Orientation, whenConvertedToString_thenCorrect) {
  ASSERT_EQ("up", orientationStringFromValue(Orientation::Up));
  ASSERT_EQ("up_mirrored", orientationStringFromValue(Orientation::UpMirrored));
  ASSERT_EQ("right", orientationStringFromValue(Orientation::Right));
  ASSERT_EQ(
      "right_mirrored", orientationStringFromValue(Orientation::RightMirrored));
  ASSERT_EQ("bottom", orientationStringFromValue(Orientation::Bottom));
  ASSERT_EQ(
      "bottom_mirrored",
      orientationStringFromValue(Orientation::BottomMirrored));
  ASSERT_EQ("left", orientationStringFromValue(Orientation::Left));
  ASSERT_EQ(
      "left_mirrored", orientationStringFromValue(Orientation::LeftMirrored));
  ASSERT_EQ(
      "unknown (9)", orientationStringFromValue(static_cast<Orientation>(9)));
}

} // namespace test
} // namespace image
} // namespace spectrum
} // namespace facebook
