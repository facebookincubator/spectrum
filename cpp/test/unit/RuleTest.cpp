// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/Rule.h>
#include <spectrum/Recipe.h>
#include <spectrum/testutils/TestUtils.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace test {

TEST(Rule, whenCreated_thenDefaultValuesCorrect) {
  const auto rule = Rule{"rule"};

  ASSERT_EQ("rule", rule.name);
  ASSERT_TRUE(rule.allowedInputFormats.empty());
  ASSERT_TRUE(rule.allowedOutputFormats.empty());
  ASSERT_FALSE(rule.requiresEqualInputOutputFormat);
  ASSERT_FALSE(rule.isPassthrough);
  ASSERT_EQ(Rule::CropSupport::None, rule.cropSupport);
  ASSERT_EQ(Rule::ResizeSupport::None, rule.resizeSupport);
  ASSERT_EQ(Rule::RotateSupport::None, rule.rotateSupport);
}

} // namespace test
} // namespace spectrum
} // namespace facebook
