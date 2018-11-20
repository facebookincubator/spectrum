// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/matchers/All.h>
#include <spectrum/requirements/CropAbsoluteToOrigin.h>
#include <spectrum/requirements/CropRelativeToOrigin.h>

#include <spectrum/testutils/TestUtils.h>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {
namespace test {
namespace {
Result cropRequirementFailureReason(
    const Rule::CropSupport cropSupport,
    const folly::Optional<requirements::Crop>& cropRequirement) {
  const auto rule = Rule{.name = "rule", .cropSupport = cropSupport};
  auto parameters = testutils::makeDummyOperationParameters();

  parameters.transformations.cropRequirement = cropRequirement;
  return matchesCropRequirement(rule, parameters);
}
} // namespace

TEST(core_matchers_Crop, whenRequirementGiven_noneSupported_thenFail) {
  const auto cropRequirement = spectrum::requirements::CropRelativeToOrigin(
      {.top = 0.2f, .left = 0.1f, .bottom = 0.4f, .right = 0.3f}, false);
  ASSERT_EQ(
      reasons::CropUnsupported,
      cropRequirementFailureReason(Rule::CropSupport::None, cropRequirement)
          .failureReason);
}

TEST(
    core_matchers_Crop,
    whenCroppingIsGivenAndExact_butOnlyApproximateSupported_thenFail) {
  const auto cropRequirement = spectrum::requirements::CropRelativeToOrigin(
      {.top = 0.2f, .left = 0.1f, .bottom = 0.4f, .right = 0.3f}, true);
  ASSERT_EQ(
      reasons::CropUnsupported,
      cropRequirementFailureReason(
          Rule::CropSupport::Approximate, cropRequirement)
          .failureReason);
}

TEST(
    core_matchers_Crop,
    whenCroppingIsGivenAndExact_exactSupported_thenSucceed) {
  const auto cropRequirement = spectrum::requirements::CropRelativeToOrigin(
      {.top = 0.2f, .left = 0.1f, .bottom = 0.4f, .right = 0.3f}, true);
  ASSERT_TRUE(
      cropRequirementFailureReason(Rule::CropSupport::Exact, cropRequirement)
          .success());
}

TEST(
    core_matchers_Crop,
    whenCroppingIsNotGiven_andApproximateSupported_thenSucceed) {
  ASSERT_TRUE(cropRequirementFailureReason(Rule::CropSupport::None, folly::none)
                  .success());
}

} // namespace test
} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
