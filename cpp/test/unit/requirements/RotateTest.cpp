// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/requirements/Rotate.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::requirements;

namespace facebook {
namespace spectrum {
namespace requirements {
namespace test {

TEST(Rotate, whenCreatingWithDefaults_thenValuesCorrect) {
  const auto requirement = Rotate{};

  ASSERT_EQ(0, requirement.degrees);
  ASSERT_FALSE(requirement.flipHorizontally);
  ASSERT_FALSE(requirement.flipVertically);
  ASSERT_FALSE(requirement.forceUpOrientation);
}

TEST(Rotate, whenComparingWithDifferentValues_thenNotEqual) {
  ASSERT_NE(Rotate({.degrees = 90}), Rotate());
  ASSERT_NE(Rotate({.flipVertically = true}), Rotate());
  ASSERT_NE(Rotate({.flipHorizontally = true}), Rotate());
  ASSERT_NE(Rotate({.forceUpOrientation = true}), Rotate());
}

TEST(Rotate, whenDegreesIsLowerThan0OrGreater359_thenSanitised) {
  ASSERT_EQ(0, Rotate({.degrees = 360}).sanitisedDegrees());
  ASSERT_EQ(10, Rotate({.degrees = 370}).sanitisedDegrees());
  ASSERT_EQ(350, Rotate({.degrees = -10}).sanitisedDegrees());
}

TEST(Rotate, whenFlipped_thenFlipIsTrue) {
  ASSERT_TRUE(Rotate({.flipHorizontally = true}).flip());
  ASSERT_TRUE(Rotate({.flipVertically = true}).flip());
}

TEST(Rotate, whenNotFlipped_thenFlipIsFalse) {
  ASSERT_FALSE(Rotate().flip());
}

TEST(Rotate, whenZeroRotation_thenNoopIsTrue) {
  ASSERT_TRUE(Rotate().noop());
}

TEST(Rotate, whenNonZeroRotation_thenNoopIsFalse) {
  ASSERT_FALSE(Rotate({.degrees = 90}).noop());
  ASSERT_FALSE(Rotate({.flipHorizontally = true}).noop());
  ASSERT_FALSE(Rotate({.flipVertically = true}).noop());
  ASSERT_FALSE(Rotate({.forceUpOrientation = true}).noop());
}

} // namespace test
} // namespace requirements
} // namespace spectrum
} // namespace facebook
