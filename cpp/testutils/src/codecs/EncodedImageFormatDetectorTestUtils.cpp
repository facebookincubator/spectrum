// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "EncodedImageFormatDetectorTestUtils.h"

namespace facebook {
namespace spectrum {
namespace codecs {
namespace testutils {

EncodedImageFormatDetectorHandler
makeEncodedImageFormatDetectorHandlerReturning(
    const folly::Optional<image::EncodedFormat>& format,
    bool& didRunHandler) {
  didRunHandler = false;
  return [&didRunHandler, format](io::IImageSource& /* unused */) {
    didRunHandler = true;
    return format;
  };
}

} // namespace testutils
} // namespace codecs
} // namespace spectrum
} // namespace facebook
