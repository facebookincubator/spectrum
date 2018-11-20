// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "PixelSpecificationDecision.h"

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {

image::pixel::Specification calculateOutputPixelSpecification(
    const image::Specification& inputImageSpecification,
    const folly::Optional<image::pixel::Specification>&
        outputImagePixelSpecificationRequirement,
    const codecs::CompressorProvider::PixelSpecificationNarrower&
        pixelSpecificationNarrower) {
  if (outputImagePixelSpecificationRequirement.hasValue()) {
    // Observe override if given
    return *outputImagePixelSpecificationRequirement;
  } else if (pixelSpecificationNarrower != nullptr) {
    // Apply narrower if given
    return pixelSpecificationNarrower(
        inputImageSpecification.pixelSpecification);
  } else {
    // Otherwise, keep the input pixel specification
    return inputImageSpecification.pixelSpecification;
  }
}

} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
