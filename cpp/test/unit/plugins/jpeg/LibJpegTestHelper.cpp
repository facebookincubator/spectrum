// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegTestHelper.h"

#include <spectrum/plugins/jpeg/LibJpegDecompressor.h>

#include <memory>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {
namespace testutils {

::testing::AssertionResult assertOutputValidJpeg(
    io::testutils::FakeImageSink& sink,
    const image::Size& imageSize) {
  // inspect output
  const std::string output = sink.stringContent();
  if (output.size() == 0) {
    return ::testing::AssertionFailure() << "output is empty";
  }

  // SOI
  if ('\xFF' != output[0] || '\xD8' != output[1]) {
    return ::testing::AssertionFailure()
        << "output does not start with 0xFF 0xD8 (SOI)";
  }

  // EOI
  if ('\xFF' != output[output.size() - 2] ||
      '\xD9' != output[output.size() - 1]) {
    return ::testing::AssertionFailure()
        << "output does not end with 0xFF 0xD9 (EOI)";
  }

  // re-read into jpeg decompressor and validate header information
  auto source = io::testutils::makeVectorImageSource(output);
  auto decompressor = LibJpegDecompressor{source};

  const auto newImageSize = decompressor.outputImageSpecification().size;
  if (newImageSize.width != imageSize.width ||
      newImageSize.height != imageSize.height) {
    return ::testing::AssertionFailure()
        << "reading the generated output lead to different size: "
        << newImageSize.width << "x" << newImageSize.height;
  }

  return ::testing::AssertionSuccess();
}

} // namespace testutils
} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
