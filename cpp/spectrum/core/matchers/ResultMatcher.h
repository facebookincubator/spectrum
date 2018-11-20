// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <folly/Optional.h>
#include <folly/Range.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {

struct Result {
  folly::Optional<folly::StringPiece> failureReason;

  Result() = default;
  Result(const folly::StringPiece& failureReason);
  bool success() const;

  static Result ok();
};

} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
