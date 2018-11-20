// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ResultMatcher.h"

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {

Result::Result(const folly::StringPiece& failureReason)
    : failureReason(failureReason) {}

bool Result::success() const {
  return !failureReason.hasValue();
}

Result Result::ok() {
  return Result();
}

} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
