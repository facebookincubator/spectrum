// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "All.h"

#include <spectrum/core/utils/Numeric.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {
namespace reasons {
const folly::StringPiece RotateUnsupported{
    "characteristic_matcher_rotate_unsupported"};
}

namespace {
bool _matchesRotateRequirement(
    const Rule::RotateSupport rotateSupport,
    const spectrum::requirements::Rotate& rotateRequirement) {
  const auto isRotationMultipleOf90 =
      numeric::isMultipleOf90(rotateRequirement.degrees);
  const auto isRotationMultipleOf180 =
      numeric::isMultipleOf180(rotateRequirement.degrees);

  switch (rotateSupport) {
    case Rule::RotateSupport::None:
      // Whether there's a forced rotation (assigned degrees or flip) or a
      // potential rotation (forceUpOrientation) we exclude non-noop rotate
      // requirements from rules that cannot rotate.
      return rotateRequirement.noop();
    case Rule::RotateSupport::MultipleOf90:
      // Excluding forceUpOrientation as the orientation may derive from
      // metadata which can be flipped.
      return isRotationMultipleOf90 && !rotateRequirement.flip() &&
          !rotateRequirement.forceUpOrientation;
    case Rule::RotateSupport::MultipleOf180:
      return isRotationMultipleOf180 && !rotateRequirement.flip() &&
          !rotateRequirement.forceUpOrientation;
    case Rule::RotateSupport::MultipleOf90Flip:
      return isRotationMultipleOf90;
  }
}
} // namespace

Result matchesRotateRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters) {
  const auto& rotateRequirement = parameters.transformations.rotateRequirement;

  if (rotateRequirement.hasValue() &&
      !_matchesRotateRequirement(rule.rotateSupport, *rotateRequirement)) {
    return reasons::RotateUnsupported;
  }

  return Result::ok();
}

} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
