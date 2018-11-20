// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Options.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <atomic>
#include <exception>
#include <string>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {

Options::Options(
    const folly::Optional<requirements::Encode>& encodeRequirement,
    const Transformations& transformations,
    const folly::Optional<image::Metadata>& metadata,
    const Configuration& configuration,
    const folly::Optional<image::pixel::Specification>&
        outputPixelSpecificationRequirement)
    : encodeRequirement(encodeRequirement),
      transformations(transformations),
      metadata(metadata),
      outputPixelSpecificationRequirement(outputPixelSpecificationRequirement),

      configuration(configuration){};

bool Options::operator==(const Options& rhs) const {
  return encodeRequirement == rhs.encodeRequirement &&
      outputPixelSpecificationRequirement ==
      rhs.outputPixelSpecificationRequirement &&
      transformations == rhs.transformations &&
      configuration == rhs.configuration &&
      metadata == rhs.metadata;
}

image::Format Options::outputFormat() const {
  return encodeRequirement.hasValue() ? encodeRequirement->format
                                      : image::formats::Bitmap;
}

DecodeOptions::DecodeOptions(
    const Transformations& transformations,
    const Configuration& configuration,
    const folly::Optional<image::pixel::Specification>&
        outputPixelSpecificationRequirement)
    : Options(
          folly::none,
          transformations,
          folly::none,
          configuration,
          outputPixelSpecificationRequirement) {}

EncodeOptions::EncodeOptions(
    const requirements::Encode& encodeRequirement,
    const Transformations& transformations,
    const folly::Optional<image::Metadata>& metadata,
    const Configuration& configuration,
    const folly::Optional<image::pixel::Specification>&
        outputPixelSpecificationRequirement)
    : Options(
          encodeRequirement,
          transformations,
          metadata,
          configuration,
          outputPixelSpecificationRequirement) {}

TranscodeOptions::TranscodeOptions(
    const requirements::Encode& encodeRequirement,
    const Transformations& transformations,
    const folly::Optional<image::Metadata>& metadata,
    const Configuration& configuration,
    const folly::Optional<image::pixel::Specification>&
        outputPixelSpecificationRequirement)
    : Options(
          encodeRequirement,
          transformations,
          metadata,
          configuration,
          outputPixelSpecificationRequirement) {}

TransformOptions::TransformOptions(
    const Transformations& transformations,
    const Configuration& configuration,
    const folly::Optional<image::pixel::Specification>&
        outputPixelSpecificationRequirement)
    : Options(
          folly::none,
          transformations,
          folly::none,
          configuration,
          outputPixelSpecificationRequirement) {}

} // namespace spectrum
} // namespace facebook
