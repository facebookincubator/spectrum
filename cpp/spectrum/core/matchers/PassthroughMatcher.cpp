// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "All.h"

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {
namespace reasons {
const folly::StringPiece PassthroughDenied{
    "characteristic_matcher_passthrough_denied"};
}

namespace {
bool _matchesPassthroughEncodeRequirement(
    const requirements::Encode& encodeRequirement) {
  switch (encodeRequirement.mode) {
    case requirements::Encode::Mode::Lossless:
    case requirements::Encode::Mode::Any:
      return true;
    case requirements::Encode::Mode::Lossy:
      return false;
  }
}
} // namespace

Result matchesPassthroughRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters) {
  const auto& encodeRequirement = parameters.encodeRequirement;
  if (rule.isPassthrough && encodeRequirement.hasValue() &&
      !_matchesPassthroughEncodeRequirement(*encodeRequirement)) {
    return reasons::PassthroughDenied;
  }

  return Result::ok();
}

} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
