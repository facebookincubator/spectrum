// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "RuleMatcher.h"

namespace facebook {
namespace spectrum {
namespace core {

namespace error {
const folly::StringPiece NoMatchingRule{"no_matching_rule"};
}

RuleMatcher::RuleMatcher(
    std::vector<Rule>&& rules,
    std::vector<RuleRequirementMatcher>&& requirementMatchers)
    : _rules(std::move(rules)),
      _requirementMatchers(std::move(requirementMatchers)) {}

Rule RuleMatcher::findFirstMatching(
    const Operation::Parameters& parameters) const {
  for (const auto& rule : _rules) {
    if (_matchesRequirements(rule, parameters).success()) {
      return rule;
    }
  }

  SPECTRUM_ERROR(error::NoMatchingRule);
}

matchers::Result RuleMatcher::_matchesRequirements(
    const Rule& rule,
    const Operation::Parameters& parameters) const {
  for (const auto& requirementMatchers : _requirementMatchers) {
    const auto result = requirementMatchers(rule, parameters);
    if (!result.success()) {
      return result;
    }
  }

  return matchers::Result::ok();
}

} // namespace core
} // namespace spectrum
} // namespace facebook
