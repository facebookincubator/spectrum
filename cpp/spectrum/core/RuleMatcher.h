// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Rule.h>
#include <spectrum/core/Operation.h>
#include <spectrum/core/RuleRequirementMatcher.h>

#include <vector>

namespace facebook {
namespace spectrum {
namespace core {

namespace error {
extern const folly::StringPiece NoMatchingRule;
}

class RuleMatcher {
 public:
  /**
   * Creates a new rule matcher.
   *
   * @param rules The rules to use.
   * @param requirementMatchers The requirement matchers to use.
   */
  RuleMatcher(
      std::vector<Rule>&& rules,
      std::vector<RuleRequirementMatcher>&& requirementMatchers =
          matchers::makeAll());

  /**
   * Returns the first matching rule that can handle the given transcode
   * options. Throws error::NoMatchingRule if no rule matches.
   */
  Rule findFirstMatching(const Operation::Parameters& parameters) const;

 private:
  /**
   * All the rules.
   */
  std::vector<Rule> _rules;

  /**
   * All handlers to apply against rules.
   */
  std::vector<RuleRequirementMatcher> _requirementMatchers;

  /**
   * Tests if a rule matches all characteristics.
   *
   * @param rule The rule to verify.
   * @param parameters The operation's parameters.
   * @return folly::none if it matches, or the reason of failure if it doesn't.
   */
  matchers::Result _matchesRequirements(
      const Rule& rule,
      const Operation::Parameters& parameters) const;
};

} // namespace core
} // namespace spectrum
} // namespace facebook
