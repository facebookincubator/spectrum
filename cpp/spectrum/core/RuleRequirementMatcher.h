// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Rule.h>
#include <spectrum/core/Operation.h>

#include <spectrum/core/matchers/ResultMatcher.h>

namespace facebook {
namespace spectrum {
namespace core {

using RuleRequirementMatcher = std::function<matchers::Result(
    const Rule& rule,
    const Operation::Parameters& parameters)>;

namespace matchers {
std::vector<RuleRequirementMatcher> makeAll();
}

} // namespace core
} // namespace spectrum
} // namespace facebook
