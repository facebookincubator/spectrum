// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/plugins/webp/LibWebpDecompressor.h>
#include <spectrum/io/FileImageSource.h>
#include <spectrum/testutils/TestUtils.h>
#include <array>
#include <cstddef>
#include <memory>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace webp {
namespace test {

TEST(
    plugins_webp_LibWebpDecompressor,
    whenConstructed_nothingObservableHappens) {
  io::FileImageSource source{
      testdata::paths::webp::s128x85_RGB_LOSSY.normalized()};
  auto decompressor = LibWebpDecompressor{source};
}

//
// Header information
//

TEST(
    plugins_webp_LibWebpDecompressor,
    whenReading_rgbLossy_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::webp::s128x85_RGB_LOSSY.normalized()};
  auto decompressor = LibWebpDecompressor{source};
  const auto outputImageSpecification = decompressor.outputImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::RGB,
      outputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{128, 85}), outputImageSpecification.size);
}

TEST(
    plugins_webp_LibWebpDecompressor,
    whenReading_rgbLossless_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::webp::s128x85_RGB_LOSSLESS.normalized()};
  auto decompressor = LibWebpDecompressor{source};
  const auto outputImageSpecification = decompressor.outputImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::RGB,
      outputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{128, 85}), outputImageSpecification.size);
}

TEST(
    plugins_webp_LibWebpDecompressor,
    whenReading_argbLossy_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::webp::s400x301_WITH_ALPHA_LOSSY.normalized()};
  auto decompressor = LibWebpDecompressor{source};
  const auto outputImageSpecification = decompressor.outputImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::ARGB,
      outputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{400, 301}), outputImageSpecification.size);
}

TEST(
    plugins_webp_LibWebpDecompressor,
    whenReading_argbLossless_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::webp::s400x301_WITH_ALPHA_LOSSLESS.normalized()};
  auto decompressor = LibWebpDecompressor{source};
  const auto outputImageSpecification = decompressor.outputImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::ARGB,
      outputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{400, 301}), outputImageSpecification.size);
}

//
// Reading scanlines
//

TEST(
    plugins_webp_LibWebpDecompressor,
    readingCompleteImage_PATH_WEBP_400x301_WITH_ALPHA_LOSSY) {
  io::FileImageSource source{
      testdata::paths::webp::s400x301_WITH_ALPHA_LOSSY.normalized()};
  auto decompressor = LibWebpDecompressor{source};
  const auto outputImageSpecification = decompressor.outputImageSpecification();

  for (int i = 0; i < outputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
  }
}

TEST(
    plugins_webp_LibWebpDecompressor,
    readingScanline_PATH_WEBP_16x16_cABCDEF_RGB_LOSSLESS) {
  io::FileImageSource source{
      testdata::paths::webp::s16x16_cABCDEF_RGB_LOSSLESS.normalized()};
  auto decompressor = LibWebpDecompressor{source};
  const auto outputImageSpecification = decompressor.outputImageSpecification();

  for (int i = 0; i < outputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {{0xAB, 0xCD, 0xEF}});
  }
}

TEST(
    plugins_webp_LibWebpDecompressor,
    readingScanline_PATH_WEBP_16x16_a50_cABCDEF_ARGB_LOSSLESS) {
  io::FileImageSource source{
      testdata::paths::webp::s16x16_a50_cABCDEF_ARGB_LOSSLESS.normalized()};
  auto decompressor = LibWebpDecompressor{source};
  const auto outputImageSpecification = decompressor.outputImageSpecification();

  for (int i = 0; i < outputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorArgb(
        *scanline, {{0x80, 0xAB, 0xCD, 0xEF}});
  }
}

TEST(
    plugins_webp_LibWebpDecompressor,
    readingScanlineOverrideRgb_PATH_WEBP_16x16_a50_cABCDEF_ARGB_LOSSLESS) {
  io::FileImageSource source{
      testdata::paths::webp::s16x16_a50_cABCDEF_ARGB_LOSSLESS.normalized()};
  LibWebpDecompressor decompressor{
      source, folly::none, image::pixel::specifications::RGB};
  const auto outputImageSpecification = decompressor.outputImageSpecification();

  for (int i = 0; i < outputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {{0xAB, 0xCD, 0xEF}});
  }
}

//
// Error handling
//

TEST(plugins_webp_LibWebpDecompressor, whenReadingPastScanline_thenThrows) {
  io::FileImageSource source{
      testdata::paths::webp::s16x16_a50_cABCDEF_ARGB_LOSSLESS.normalized()};
  auto decompressor = LibWebpDecompressor{source};
  const auto outputImageSpecification = decompressor.outputImageSpecification();

  for (int i = 0; i < outputImageSpecification.size.height; i++) {
    decompressor.readScanline();
  }
  ASSERT_THROW(decompressor.readScanline(), SpectrumException);
}

TEST(plugins_webp_LibWebpDecompressor, whenReadingNonWebp_thenThrows) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_cABCDEF_RGB_8B.normalized()};
  auto decompressor = LibWebpDecompressor{source};

  ASSERT_THROW(decompressor.readScanline(), SpectrumException);
}

TEST(plugins_webp_LibWebpDecompressor, whenExceedinglyLargeInput_thenThrows) {
  io::FileImageSource source{
      testdata::paths::webp::exceedingly_large.normalized()};
  auto decompressor = LibWebpDecompressor{source};

  ASSERT_SPECTRUM_THROW(
      decompressor.readScanline(), codecs::error::DecompressorFailure);
}

} // namespace test
} // namespace webp
} // namespace plugins
} // namespace spectrum
} // namespace facebook
