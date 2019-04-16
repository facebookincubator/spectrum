// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/plugins/avif/LibAvifTranscodingPlugin.h>
#include <spectrum/testutils/TestUtils.h>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {
namespace test {

namespace {

folly::Optional<image::EncodedFormat> runEncodedImageFormatHandler(
    const std::string& input) {
  auto source = io::testutils::makeVectorImageSource(input);
  const auto avifPlugin = avif::makeTranscodingPlugin();
  const auto handler = avifPlugin.encodedImageFormatDetectorHandlers.at(0);
  return handler(source);
}

} // namespace

TEST(plugins_avif_EncodedImageFormatHandler, whenGivenEmptySource_thenNone) {
  ASSERT_EQ(folly::none, runEncodedImageFormatHandler(""));
}

TEST(plugins_avif_EncodedImageFormatHandler, whenWrongMagic_thenNone) {
  const auto input = std::string(
      "ABCD"
      "\x00\x00\x00\x00" // version, length
      "AV01"
      "\x00\x00\x00\x00" // width, height
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // frame nom, dem, cnt
      "\x00\x00\x00\x00", // unused
      32);
  ASSERT_EQ(folly::none, runEncodedImageFormatHandler(input));
}

TEST(plugins_avif_EncodedImageFormatHandler, whenIvfButWrongFourCC_thenNone) {
  const auto input = std::string(
      "DKIF"
      "\x00\x00\x00\x00" // version, length
      "AV02"
      "\x00\x00\x00\x00" // width, height
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // frame nom, dem, cnt
      "\x00\x00\x00\x00", // unused
      32);
  ASSERT_EQ(folly::none, runEncodedImageFormatHandler(input));
}

TEST(plugins_avif_EncodedImageFormatHandler, whenCorrectFourCC_thenIvfAv1) {
  const auto input = std::string(
      "DKIF"
      "\x00\x00\x00\x00" // version, length
      "AV01"
      "\x00\x00\x00\x00" // width, height
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" // frame nom, dem, cnt
      "\x00\x00\x00\x00", // unused
      32);
  ASSERT_EQ(formats::IvfAv1, runEncodedImageFormatHandler(input));
}

} // namespace test
} // namespace avif
} // namespace plugins
} // namespace spectrum
} // namespace facebook
