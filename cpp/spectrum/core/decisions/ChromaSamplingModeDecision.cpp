// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ChromaSamplingModeDecision.h"

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {
namespace {

bool chromaSamplingModeVectorContains(
    const std::vector<image::ChromaSamplingMode>& supportedChromaSamplingModes,
    const image::ChromaSamplingMode candidate) {
  return std::find(
             supportedChromaSamplingModes.cbegin(),
             supportedChromaSamplingModes.cend(),
             candidate) != supportedChromaSamplingModes.cend();
}
} // namespace

image::ChromaSamplingMode calculateOutputChromaSamplingMode(
    const image::Specification& inputImageSpecification,
    const std::vector<image::ChromaSamplingMode>&
        supportedTargetChromaSamplingModes,
    const Configuration& configuration) {
  const auto chromaSamplingModeOverride =
      Configuration::General::chromaSamplingModeOverrideToChromaSamplingMode(
          configuration.general.chromaSamplingModeOverride());

  if (chromaSamplingModeOverride.hasValue()) {
    // There's an override - let's use it
    SPECTRUM_ENFORCE_IF_NOT(chromaSamplingModeVectorContains(
        supportedTargetChromaSamplingModes, *chromaSamplingModeOverride));
    return *chromaSamplingModeOverride;
  } else if (
      inputImageSpecification.chromaSamplingMode.hasValue() &&
      configuration.jpeg.propagateSamplingModeFromSource() &&
      chromaSamplingModeVectorContains(
          supportedTargetChromaSamplingModes,
          *inputImageSpecification.chromaSamplingMode)) {
    // We can propagate the sampling mode, and the compressor supports the
    // source chroma sampling mode.
    return *inputImageSpecification.chromaSamplingMode;
  } else {
    SPECTRUM_ENFORCE_IF(supportedTargetChromaSamplingModes.empty());
    return supportedTargetChromaSamplingModes[0];
  }
}

} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
