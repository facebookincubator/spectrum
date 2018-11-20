// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "BaseDecision.h"

#include <spectrum/core/decisions/ChromaSamplingModeDecision.h>
#include <spectrum/core/decisions/MetadataDecision.h>
#include <spectrum/core/decisions/PixelSpecificationDecision.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {
namespace {

image::Specification _calculateOutputImageSpecification(
    const Operation& operation,
    const OrientationDecision& orientation,
    const ResizeDecision& resize) {
  const auto& parameters = operation.parameters;
  const auto& codecs = operation.codecs;
  return {
      .size = orientation.size,
      .format = parameters.outputImageFormat,
      .pixelSpecification = calculateOutputPixelSpecification(
          parameters.inputImageSpecification,
          parameters.outputPixelSpecificationRequirement,
          codecs.compressorProvider.pixelSpecificationNarrower),
      .orientation = orientation.metadataOrientation,
      .chromaSamplingMode = calculateOutputChromaSamplingMode(
          parameters.inputImageSpecification,
          codecs.compressorProvider.supportedChromaSamplingModes,
          operation.configuration),
      .metadata = calculateOutputMetadata(
          parameters.inputImageSpecification,
          parameters.extraMetadata,
          orientation.metadataOrientation,
          codecs.compressorProvider.supportsSettingMetadata),
  };
}
} // namespace

BaseDecision BaseDecision::calculate(const Operation& operation) {
  const auto& parameters = operation.parameters;
  const auto& codecs = operation.codecs;

  const auto resize = calculateResizeDecision(
      parameters.inputImageSpecification.size,
      parameters.transformations.resizeRequirement,
      codecs.decompressorProvider.supportedSamplingRatios,
      parameters.transformations.cropRequirement);
  const auto orientation = OrientationDecision::calculate(
      parameters.transformations.rotateRequirement,
      parameters.inputImageSpecification.orientation,
      resize.sizeAfterScaling(),
      operation.configuration,
      codecs.compressorProvider.supportsSettingMetadata);

  return BaseDecision{
      .resize = resize,
      .orientation = orientation,
      .outputImageSpecification =
          _calculateOutputImageSpecification(operation, orientation, resize),
  };
}

} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
