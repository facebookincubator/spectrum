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

Result resizeRequirementFailureReason(
    const Rule::ResizeSupport resizeSupport,
    const folly::Optional<spectrum::requirements::Resize::Mode>&
        resizeRequirementMode) {
  const auto rule = Rule{.name = "rule", .resizeSupport = resizeSupport};
  auto parameters = testutils::makeDummyOperationParameters();

  if (resizeRequirementMode.hasValue()) {
    parameters.transformations.resizeRequirement =
        spectrum::requirements::Resize{
            .mode = *resizeRequirementMode,
            .targetSize = {200, 300},
        };
  }

  return matchesResizeRequirement(rule, parameters);
}
} // namespace

TEST(core_matchers_Resize, whenNoneRequirement_thenSucceed) {
  ASSERT_TRUE(
      resizeRequirementFailureReason(Rule::ResizeSupport::None, folly::none)
          .success());

  ASSERT_TRUE(
      resizeRequirementFailureReason(Rule::ResizeSupport::Exact, folly::none)
          .success());

  ASSERT_TRUE(resizeRequirementFailureReason(
                  Rule::ResizeSupport::ExactOrSmaller, folly::none)
                  .success());

  ASSERT_TRUE(resizeRequirementFailureReason(
                  Rule::ResizeSupport::ExactOrLarger, folly::none)
                  .success());
}

TEST(core_matchers_Resize, whenNoneResizeSupportAnyRequirement_thenFail) {
  ASSERT_EQ(
      reasons::ResizeUnsupported,
      resizeRequirementFailureReason(
          Rule::ResizeSupport::None,
          spectrum::requirements::Resize::Mode::Exact)
          .failureReason);

  ASSERT_EQ(
      reasons::ResizeUnsupported,
      resizeRequirementFailureReason(
          Rule::ResizeSupport::None,
          spectrum::requirements::Resize::Mode::ExactOrSmaller)
          .failureReason);

  ASSERT_EQ(
      reasons::ResizeUnsupported,
      resizeRequirementFailureReason(
          Rule::ResizeSupport::None,
          spectrum::requirements::Resize::Mode::ExactOrLarger)
          .failureReason);
}

TEST(core_matchers_Resize, whenExactResizeSupportAnyRequirement_thenSucceed) {
  ASSERT_TRUE(resizeRequirementFailureReason(
                  Rule::ResizeSupport::Exact,
                  spectrum::requirements::Resize::Mode::Exact)
                  .success());

  ASSERT_TRUE(resizeRequirementFailureReason(
                  Rule::ResizeSupport::Exact,
                  spectrum::requirements::Resize::Mode::ExactOrLarger)
                  .success());

  ASSERT_TRUE(resizeRequirementFailureReason(
                  Rule::ResizeSupport::Exact,
                  spectrum::requirements::Resize::Mode::ExactOrSmaller)
                  .success());
}

TEST(
    core_matchers_Resize,
    whenExactOrSmallerResizeSupportExactOrSmallerRequirement_thenSucceed) {
  ASSERT_TRUE(resizeRequirementFailureReason(
                  Rule::ResizeSupport::ExactOrSmaller,
                  spectrum::requirements::Resize::Mode::ExactOrSmaller)
                  .success());
}

TEST(
    core_matchers_Resize,
    whenExactOrSmallerResizeSupportAnyNonExactOrSmallerRequirement_thenFail) {
  ASSERT_EQ(
      reasons::ResizeUnsupported,
      resizeRequirementFailureReason(
          Rule::ResizeSupport::ExactOrSmaller,
          spectrum::requirements::Resize::Mode::Exact)
          .failureReason);

  ASSERT_EQ(
      reasons::ResizeUnsupported,
      resizeRequirementFailureReason(
          Rule::ResizeSupport::ExactOrSmaller,
          spectrum::requirements::Resize::Mode::ExactOrLarger)
          .failureReason);
}

TEST(
    core_matchers_Resize,
    whenExactOrLargerResizeSupportExactOrLargerRequirement_thenSucceed) {
  ASSERT_TRUE(resizeRequirementFailureReason(
                  Rule::ResizeSupport::ExactOrLarger,
                  spectrum::requirements::Resize::Mode::ExactOrLarger)
                  .success());
}

TEST(
    core_matchers_Resize,
    whenExactOrLargerResizeSupportAnyNonExactOrLargerRequirement_thenFail) {
  ASSERT_EQ(
      reasons::ResizeUnsupported,
      resizeRequirementFailureReason(
          Rule::ResizeSupport::ExactOrLarger,
          spectrum::requirements::Resize::Mode::Exact)
          .failureReason);

  ASSERT_EQ(
      reasons::ResizeUnsupported,
      resizeRequirementFailureReason(
          Rule::ResizeSupport::ExactOrLarger,
          spectrum::requirements::Resize::Mode::ExactOrSmaller)
          .failureReason);
}

} // namespace test
} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
