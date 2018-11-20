// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "RuleRequirementMatcher.h"

#include <spectrum/core/matchers/All.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {

std::vector<RuleRequirementMatcher> makeAll() {
  return std::vector<RuleRequirementMatcher>{
      &matchesPassthroughRequirement,
      &matchesAllowedInputImageFormat,
      &matchesAllowedOutputImageFormats,
      &matchesEqualInputOutputImageFormatRequirement,
      &matchesResizeRequirement,
      &matchesRotateRequirement,
      &matchesCropRequirement,
  };
}

} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
