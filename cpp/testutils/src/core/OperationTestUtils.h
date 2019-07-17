// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/Operation.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace testutils {

/**
 * Creates a dummy operation specification.
 */
Operation::Parameters makeDummyOperationParameters(
    const image::Format& imageFormat = image::formats::Bitmap,
    const folly::Optional<image::pixel::Specification>&
        outputPixelSpecificationRequirement = folly::none);

/**
 * Creates an operation from the source / sink with dummy data.
 */
Operation makeOperationFromIO(io::IImageSource& source, io::IImageSink& sink);

} // namespace testutils
} // namespace core
} // namespace spectrum
} // namespace facebook
