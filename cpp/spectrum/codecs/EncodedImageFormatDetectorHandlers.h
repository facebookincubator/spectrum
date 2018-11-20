// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Format.h>
#include <spectrum/io/IImageSource.h>

#include <folly/Optional.h>
#include <folly/Range.h>

#include <functional>
#include <vector>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace error {
extern const folly::StringPiece EncodedImageFormatDetectorHandlerEmptyHeader;
} // namespace error

/**
 * The signature for a Handler.
 *
 * @param source The source encapsulating the image which needs detection.
 * @return The identified image format, none otherwise.
 */
using EncodedImageFormatDetectorHandler =
    std::function<folly::Optional<image::EncodedFormat>(
        io::IImageSource& source)>;

EncodedImageFormatDetectorHandler makeJpegImageFormatDetectorHandler();
EncodedImageFormatDetectorHandler makePngImageFormatDetectorHandler();
EncodedImageFormatDetectorHandler makeGif87ImageFormatDetectorHandler();
EncodedImageFormatDetectorHandler makeGif89ImageFormatDetectorHandler();
EncodedImageFormatDetectorHandler makeWebpVp8ImageFormatDetectorHandler();
EncodedImageFormatDetectorHandler makeWebpVp8lImageFormatDetectorHandler();
EncodedImageFormatDetectorHandler makeWebpVp8xImageFormatDetectorHandler();
EncodedImageFormatDetectorHandler makeHeifImageFormatDetectorHandler();

std::vector<EncodedImageFormatDetectorHandler>
makeAllImageFormatDetectorHandlers();

EncodedImageFormatDetectorHandler makeSimpleImageFormatDetectorHandler(
    const folly::StringPiece& header,
    const image::EncodedFormat& imageFormat);

} // namespace codecs
} // namespace spectrum
} // namespace facebook
