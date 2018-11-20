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
const folly::StringPiece ResizeUnsupported{
    "characteristic_matcher_resize_unsupported"};
}

namespace {
bool _matchesResizeRequirement(
    const Rule::ResizeSupport supportedResize,
    const spectrum::requirements::Resize& resizeRequirement) {
  switch (supportedResize) {
    case Rule::ResizeSupport::None:
      return false;
    case Rule::ResizeSupport::Exact:
      return true;
    case Rule::ResizeSupport::ExactOrLarger:
      return resizeRequirement.mode ==
          spectrum::requirements::Resize::Mode::ExactOrLarger;
    case Rule::ResizeSupport::ExactOrSmaller:
      return resizeRequirement.mode ==
          spectrum::requirements::Resize::Mode::ExactOrSmaller;
  }
}
} // namespace

Result matchesResizeRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters) {
  const auto& resizeRequirement = parameters.transformations.resizeRequirement;

  if (resizeRequirement.hasValue()) {
    if (!_matchesResizeRequirement(rule.resizeSupport, *resizeRequirement)) {
      return reasons::ResizeUnsupported;
    }
  }

  return Result::ok();
}

} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
