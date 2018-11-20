// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/EncodedImageFormatDetector.h>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace testutils {

/**
 * Creates an encoded image format detector handler that returns the specified
 * image format.
 *
 * @param format The encoded image format to return.
 * @param didRunHandler A boolean reference set to true when the handler is
 * called.
 */
EncodedImageFormatDetectorHandler
makeEncodedImageFormatDetectorHandlerReturning(
    const folly::Optional<image::EncodedFormat>& format,
    bool& didRunHandler);

} // namespace testutils
} // namespace codecs
} // namespace spectrum
} // namespace facebook
