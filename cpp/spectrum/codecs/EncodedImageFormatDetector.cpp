// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "EncodedImageFormatDetector.h"

#include <spectrum/io/RewindableImageSource.h>

#include <folly/Range.h>

#include <cstring>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace error {
const folly::StringPiece EncodedImageFormatNotDetected{
    "encoded_format_not_detected"};
} // namespace error

EncodedImageFormatDetector::EncodedImageFormatDetector(
    std::vector<EncodedImageFormatDetectorHandler>&& handlers)
    : _handlers(std::move(handlers)) {}

image::EncodedFormat EncodedImageFormatDetector::detectFormat(
    io::RewindableImageSource& source) const {
  for (auto& handler : _handlers) {
    source.mark();
    const auto imageFormat = handler(source);
    source.reset();

    if (imageFormat.hasValue()) {
      return *imageFormat;
    }
  }

  SPECTRUM_ERROR(error::EncodedImageFormatNotDetected);
}

} // namespace codecs
} // namespace spectrum
} // namespace facebook
