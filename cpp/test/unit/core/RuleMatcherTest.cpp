// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/RuleMatcher.h>
#include <spectrum/testutils/TestUtils.h>

#include <folly/FixedString.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace test {
namespace {

matchers::Result makeValidFailureResult() {
  return matchers::Result("reason");
}

RuleRequirementMatcher makeCharacteristicMatcher(
    int& callCount,
    const matchers::Result& result,
    const int onlyReturnAfter = 0) {
  return [&callCount, result, onlyReturnAfter](
             const Rule&, const Operation::Parameters&) -> matchers::Result {
    callCount++;

    const auto isGreaterThanCallCount = callCount >= onlyReturnAfter + 1;
    if (result.success()) {
      return isGreaterThanCallCount ? result : makeValidFailureResult();
    } else {
      return isGreaterThanCallCount ? result : matchers::Result::ok();
    }
  };
}
} // namespace

TEST(core_RuleMatcher, whenRuleOnlyMatchesPastSecond_thenSecondReturned) {
  auto functorCallCount = int{0};
  const auto functor =
      makeCharacteristicMatcher(functorCallCount, matchers::Result::ok(), 1);
  const auto parameters = testutils::makeDummyOperationParameters();
  const auto ruleMatcher = RuleMatcher(
      {{.name = "rule1"}, {.name = "rule2"}, {.name = "rule3"}}, {functor});

  ASSERT_EQ("rule2", ruleMatcher.findFirstMatching(parameters).name);
  ASSERT_EQ(2, functorCallCount);
}

TEST(core_RuleMatcher, whenNoRulesMatches_thenThrows) {
  auto functorCallCount = int{0};
  const auto functor =
      makeCharacteristicMatcher(functorCallCount, makeValidFailureResult());
  const auto parameters = testutils::makeDummyOperationParameters();
  const auto ruleMatcher =
      RuleMatcher({{.name = "rule1"}, {.name = "rule2"}}, {functor});

  ASSERT_SPECTRUM_THROW(
      ruleMatcher.findFirstMatching(parameters),
      spectrum::core::error::NoMatchingRule);
  ASSERT_EQ(2, functorCallCount);
}

} // namespace test
} // namespace core
} // namespace spectrum
} // namespace facebook
