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
Result passthroughFailureReason(
    const bool isRulePassthrough,
    const folly::Optional<requirements::Encode::Mode>& encodeRequirementMode,
    const folly::Optional<image::Metadata>& extraMetadata = folly::none) {
  auto parameters =
      testutils::makeDummyOperationParameters(image::formats::Jpeg);
  parameters.encodeRequirement = requirements::Encode{
      .format = image::formats::Jpeg,
      .mode = encodeRequirementMode.value_or(requirements::Encode::Mode::Any),
  };
  parameters.extraMetadata = extraMetadata;

  const auto rule = Rule{.name = "rule", .isPassthrough = isRulePassthrough};
  return matchesPassthroughRequirement(rule, parameters);
}
} // namespace

image::Metadata makeMetadata() {
  return image::Metadata{{}, image::metadata::ICCProfile()};
}

TEST(core_matchers_Passthrough, whenIsPassthrough_mustTranscode_thenFail) {
  ASSERT_EQ(
      reasons::PassthroughDenied,
      passthroughFailureReason(true, requirements::Encode::Mode::Lossy)
          .failureReason);
}

TEST(core_matchers_Passthrough, whenIsPassthrough_hasMetadata_thenFail) {
  ASSERT_EQ(
      reasons::PassthroughDenied,
      passthroughFailureReason(
          true, requirements::Encode::Mode::Lossless, makeMetadata())
          .failureReason);
}

TEST(core_matchers_Passthrough, whenAnyOther_thenSucceeds) {
  ASSERT_TRUE(
      passthroughFailureReason(true, requirements::Encode::Mode::Lossless)
          .success());
  ASSERT_TRUE(passthroughFailureReason(true, requirements::Encode::Mode::Any)
                  .success());
  ASSERT_TRUE(
      passthroughFailureReason(false, requirements::Encode::Mode::Lossless)
          .success());
  ASSERT_TRUE(passthroughFailureReason(false, requirements::Encode::Mode::Lossy)
                  .success());
  ASSERT_TRUE(passthroughFailureReason(false, requirements::Encode::Mode::Any)
                  .success());
}

} // namespace test
} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
