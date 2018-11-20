// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/matchers/All.h>

#include <spectrum/testutils/TestUtils.h>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {
namespace test {
namespace {
Result rotateRequirementFailureReason(
    const Rule::RotateSupport rotateSupport,
    const folly::Optional<requirements::Rotate>& rotateRequirement) {
  const auto rule = Rule{.name = "rule", .rotateSupport = rotateSupport};
  auto parameters = testutils::makeDummyOperationParameters();

  parameters.transformations.rotateRequirement = rotateRequirement;

  return matchesRotateRequirement(rule, parameters);
}
} // namespace

TEST(core_matchers_Rotate, whenNone_thenSucceed) {
  ASSERT_TRUE(
      rotateRequirementFailureReason(Rule::RotateSupport::None, folly::none)
          .success());
  ASSERT_TRUE(rotateRequirementFailureReason(
                  Rule::RotateSupport::None, requirements::Rotate())
                  .success());
}

TEST(core_matchers_Rotate, whenNone_thenFail) {
  ASSERT_EQ(
      reasons::RotateUnsupported,
      rotateRequirementFailureReason(
          Rule::RotateSupport::None, requirements::Rotate({.degrees = 180}))
          .failureReason);
  ASSERT_EQ(
      reasons::RotateUnsupported,
      rotateRequirementFailureReason(
          Rule::RotateSupport::None,
          requirements::Rotate({.flipVertically = true}))
          .failureReason);
  ASSERT_EQ(
      reasons::RotateUnsupported,
      rotateRequirementFailureReason(
          Rule::RotateSupport::None,
          requirements::Rotate({.forceUpOrientation = true}))
          .failureReason);
}

TEST(core_matchers_Rotate, whenMultipleOf90_thenSucceed) {
  ASSERT_TRUE(rotateRequirementFailureReason(
                  Rule::RotateSupport::MultipleOf90,
                  requirements::Rotate({.degrees = 270}))
                  .success());
}

TEST(core_matchers_Rotate, whenMultipleOf90_thenFail) {
  ASSERT_EQ(
      reasons::RotateUnsupported,
      rotateRequirementFailureReason(
          Rule::RotateSupport::MultipleOf90,
          requirements::Rotate({.flipVertically = true}))
          .failureReason);
  ASSERT_EQ(
      reasons::RotateUnsupported,
      rotateRequirementFailureReason(
          Rule::RotateSupport::MultipleOf90,
          requirements::Rotate({.forceUpOrientation = true}))
          .failureReason);
}

TEST(core_matchers_Rotate, whenMultipleOf90Flip_thenSucceed) {
  ASSERT_TRUE(rotateRequirementFailureReason(
                  Rule::RotateSupport::MultipleOf90Flip,
                  requirements::Rotate({.degrees = 540,
                                        .flipVertically = true,
                                        .flipHorizontally = true}))
                  .success());
}

TEST(core_matchers_Rotate, whenMultipleOf180Support_thenSucceed) {
  ASSERT_TRUE(rotateRequirementFailureReason(
                  Rule::RotateSupport::MultipleOf180,
                  requirements::Rotate({.degrees = 540}))
                  .success());
  ASSERT_TRUE(rotateRequirementFailureReason(
                  Rule::RotateSupport::MultipleOf180, requirements::Rotate({}))
                  .success());
}

TEST(core_matchers_Rotate, whenMultipleOf180Support_thenFail) {
  ASSERT_EQ(
      reasons::RotateUnsupported,
      rotateRequirementFailureReason(
          Rule::RotateSupport::MultipleOf180,
          requirements::Rotate({.degrees = 90}))
          .failureReason);
  ASSERT_EQ(
      reasons::RotateUnsupported,
      rotateRequirementFailureReason(
          Rule::RotateSupport::MultipleOf180,
          requirements::Rotate({.flipVertically = true}))
          .failureReason);
  ASSERT_EQ(
      reasons::RotateUnsupported,
      rotateRequirementFailureReason(
          Rule::RotateSupport::MultipleOf180,
          requirements::Rotate({.forceUpOrientation = true}))
          .failureReason);
}

} // namespace test
} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
