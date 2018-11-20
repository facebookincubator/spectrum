// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Specification.h>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {

image::Metadata calculateOutputMetadata(
    const image::Specification& inputImageSpecification,
    const folly::Optional<image::Metadata>& extraMetadata,
    const image::Orientation& metadataOrientation,
    const bool compressorSupportsSettingMetadata);

} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
