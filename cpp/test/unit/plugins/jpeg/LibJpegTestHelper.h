// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/testutils/TestUtils.h>

#include <memory>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {
namespace testutils {

/**
 * Asserts that the given output is likely to be a valid JPEG by checking: (a)
 * Starts with SOI tag, (b) ends with EOI tag and (c) the header parses
 * correctly and matches the given size.
 */
::testing::AssertionResult assertOutputValidJpeg(
    io::testutils::FakeImageSink& sink,
    const image::Size& imageSize);

} // namespace testutils
} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
