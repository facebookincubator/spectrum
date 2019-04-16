// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/plugins/avif/IvfAv1Decompressor.h>
#include <spectrum/io/FileImageSource.h>
#include <spectrum/plugins/avif/LibAvifTranscodingPlugin.h>
#include <spectrum/testutils/TestUtils.h>
#include <array>
#include <cstddef>
#include <memory>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {
namespace test {

TEST(
    plugins_avif_IvfAv1Decompressor,
    whenConstructed_nothingObservableHappens) {
  auto source = io::testutils::makeVectorImageSource("");
  auto decompressor = IvfAv1Decompressor{source};
}

TEST(
    plugins_avif_IvfAv1Decompressor,
    sourceImageSpecification_returnsCorrectFormatAndDimensions) {
  io::FileImageSource source{
      testdata::paths::avif::s256_170_rav1e_s420.normalized()};
  auto decompressor = IvfAv1Decompressor{source};

  const auto specifications = decompressor.sourceImageSpecification();
  ASSERT_EQ(avif::formats::IvfAv1, specifications.format);
  ASSERT_EQ((image::Size{256, 170}), specifications.size);
}

TEST(
    plugins_avif_IvfAv1Decompressor,
    whenReadingImage_thenSpecsOfScanlinesReturnedMatchExpectations) {
  io::FileImageSource source{
      testdata::paths::avif::s256_170_rav1e_s420.normalized()};
  auto decompressor = IvfAv1Decompressor{source};

  const auto specifications = decompressor.sourceImageSpecification();
  ASSERT_EQ((image::Size{256, 170}), specifications.size);

  for (auto row = 0; row < specifications.size.height; row++) {
    auto scanline = decompressor.readScanline();
    ASSERT_TRUE(scanline != nullptr);
    ASSERT_EQ(image::pixel::specifications::RGB, scanline->specification());
    ASSERT_EQ(specifications.size.width, scanline->width());
  }

  auto scanline = decompressor.readScanline();
  ASSERT_TRUE(scanline == nullptr);
}

} // namespace test
} // namespace avif
} // namespace plugins
} // namespace spectrum
} // namespace facebook
