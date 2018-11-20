// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "OrientationDecision.h"

#include <spectrum/core/SpectrumEnforce.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {
namespace {

image::Orientation _orientationWithRotateRequirement(
    const folly::Optional<requirements::Rotate>& rotateRequirement,
    const image::Orientation orientation,
    const Configuration& configuration) {
  if (!rotateRequirement.hasValue() || rotateRequirement->noop()) {
    return orientation;
  } else {
    return image::orientationRotatedAndFlipped(
        orientation,
        rotateRequirement->sanitisedDegrees(),
        rotateRequirement->flipHorizontally,
        rotateRequirement->flipVertically);
  }
}

bool _shouldStoreOrientationInMetadata(
    const folly::Optional<requirements::Rotate>& rotateRequirement,
    const Configuration& configuration,
    const bool compressorSupportsSettingMetadata) {
  const auto requirementRequiresStraightImage = rotateRequirement.hasValue()
      ? rotateRequirement->forceUpOrientation
      : false;

  return compressorSupportsSettingMetadata &&
      configuration.general.interpretMetadata() &&
      !requirementRequiresStraightImage;
}
} // namespace

image::Size OrientationDecision::metadataAwareSize() const {
  return size.oriented(metadataOrientation);
}

bool OrientationDecision::shouldRotatePixels() const {
  return orientation != image::orientationDefault;
}

OrientationDecision OrientationDecision::calculate(
    const folly::Optional<requirements::Rotate>& rotateRequirement,
    const image::Orientation orientation,
    const image::Size& imageSize,
    const Configuration& configuration,
    const bool compressorSupportsSettingMetadata) {
  const auto finalOrientation = _orientationWithRotateRequirement(
      rotateRequirement, orientation, configuration);
  const auto storeInMetadata = _shouldStoreOrientationInMetadata(
      rotateRequirement, configuration, compressorSupportsSettingMetadata);
  return OrientationDecision{
      .orientation =
          storeInMetadata ? image::orientationDefault : finalOrientation,
      .metadataOrientation =
          storeInMetadata ? finalOrientation : image::orientationDefault,
      .size =
          storeInMetadata ? imageSize : imageSize.oriented(finalOrientation),
  };
}

} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
