// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "MetadataDecision.h"

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {

image::Metadata calculateOutputMetadata(
    const image::Specification& inputImageSpecification,
    const folly::Optional<image::Metadata>& extraMetadata,
    const image::Orientation& metadataOrientation,
    const bool compressorSupportsSettingMetadata) {
  if (compressorSupportsSettingMetadata) {
    auto metadata = inputImageSpecification.metadata;

    if (extraMetadata.hasValue()) {
      metadata.merge(*extraMetadata);
    }

    metadata.entries().setOrientation(metadataOrientation);

    return metadata;
  } else {
    SPECTRUM_ENFORCE_IF_NOT(metadataOrientation == image::Orientation::Up);
    return image::Metadata{};
  }
}

} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
