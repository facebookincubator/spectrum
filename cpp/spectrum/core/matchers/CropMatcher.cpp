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
const folly::StringPiece CropUnsupported{
    "characteristic_matcher_crop_unsupported"};
}

namespace {
bool _matchesCropRequirement(
    const Rule::CropSupport cropSupport,
    const spectrum::requirements::Crop& cropRequirement) {
  switch (cropSupport) {
    case Rule::CropSupport::None:
      return false;
    case Rule::CropSupport::Exact:
      return true;
    case Rule::CropSupport::Approximate: {
      return !cropRequirement.mustBeExact;
    }
  }
}
} // namespace

Result matchesCropRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters) {
  const auto& cropRequirement = parameters.transformations.cropRequirement;

  if (cropRequirement.hasValue() &&
      !_matchesCropRequirement(rule.cropSupport, *cropRequirement)) {
    return reasons::CropUnsupported;
  }

  return Result::ok();
}

} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
