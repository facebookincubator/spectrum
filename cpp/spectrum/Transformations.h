// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/requirements/Crop.h>
#include <spectrum/requirements/Encode.h>
#include <spectrum/requirements/Resize.h>
#include <spectrum/requirements/Rotate.h>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {

/**
 * Image transformations that can be applied to an operation.
 */
struct Transformations {
  /**
   * The resize requirement to apply.
   */
  folly::Optional<requirements::Resize> resizeRequirement;

  /**
   * The rotate requirement to apply.
   */
  folly::Optional<requirements::Rotate> rotateRequirement;

  /**
   * The crop requirement to apply.
   */
  folly::Optional<requirements::Crop> cropRequirement;

  bool operator==(const Transformations& rhs) const;
};

} // namespace spectrum
} // namespace facebook
