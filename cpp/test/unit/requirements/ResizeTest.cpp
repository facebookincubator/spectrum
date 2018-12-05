// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/requirements/Resize.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::requirements;

namespace facebook {
namespace spectrum {
namespace requirements {
namespace test {

TEST(Resize, whenCreatingWithDefaults_thenValuesCorrect) {
  const auto requirement = Resize{};

  ASSERT_EQ(Resize::Mode::Exact, requirement.mode);
  ASSERT_EQ(image::sizeZero, requirement.targetSize);
}

TEST(Resize, whenComparingWithDifferentValues_thenNotEqual) {
  ASSERT_NE(Resize({.mode = Resize::Mode::ExactOrLarger}), Resize());
  ASSERT_NE(Resize({.targetSize = image::Size{1, 0}}), Resize());
}

} // namespace test
} // namespace test
} // namespace spectrum
} // namespace facebook
