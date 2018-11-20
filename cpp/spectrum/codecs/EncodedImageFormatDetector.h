// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/EncodedImageFormatDetectorHandlers.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/image/Format.h>

#include <folly/Optional.h>
#include <folly/Range.h>

#include <functional>
#include <vector>

namespace facebook {
namespace spectrum {

namespace io {
class IImageSource;
class RewindableImageSource;
} // namespace io
namespace codecs {
namespace error {
extern const folly::StringPiece EncodedImageFormatNotDetected;
} // namespace error

/**
 * Iterates through vended handlers and returns the detected encoded image
 * format, if any.
 */
class EncodedImageFormatDetector {
 public:
  /**
   * Creates a new EncodedImageFormatDetector.
   *
   * @param handlers A vector of handlers to leverage to detect an image format.
   */
  EncodedImageFormatDetector(
      std::vector<EncodedImageFormatDetectorHandler>&& handlers);

  /**
   * Detects the underlying image format of the source. Throws if no handler
   * returned a format.
   *
   * @param source A rewindable image source to detect the format from.
   * @return The encoded image format if found
   */
  image::EncodedFormat detectFormat(io::RewindableImageSource& source) const;

 private:
  std::vector<EncodedImageFormatDetectorHandler> _handlers;
};

} // namespace codecs
} // namespace spectrum
} // namespace facebook
