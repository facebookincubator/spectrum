// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/Operation.h>
#include <spectrum/core/ResizeUtil.h>
#include <spectrum/core/decisions/OrientationDecision.h>
#include <spectrum/image/Specification.h>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {

struct BaseDecision {
  ResizeDecision resize;
  decisions::OrientationDecision orientation;
  image::Specification outputImageSpecification;

  static BaseDecision calculate(const Operation& operation);
};

} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
