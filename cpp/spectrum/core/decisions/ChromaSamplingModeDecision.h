// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/codecs/CompressorProvider.h>
#include <spectrum/image/Specification.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {

image::ChromaSamplingMode calculateOutputChromaSamplingMode(
    const image::Specification& inputImageSpecification,
    const std::vector<image::ChromaSamplingMode>&
        supportedTargetChromaSamplingModes,
    const Configuration& configuration);

} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
