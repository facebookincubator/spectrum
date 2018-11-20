// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/plugins/jpeg/LibJpegDecompressor.h>
#include <spectrum/Configuration.h>
#include <spectrum/io/FileImageSource.h>
#include <spectrum/testutils/TestUtils.h>
#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {
namespace test {

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenConstructed_nothingObservableHappens) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto decompressor = LibJpegDecompressor{source};
}

//
// Header information
//

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingInputSize_correctValueReturned) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto decompressor = LibJpegDecompressor{source};
  const auto inputImageSize = decompressor.sourceImageSpecification().size;

  ASSERT_EQ(image::Size({128, 85}), inputImageSize);
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingOutputSize_correctValueReturned) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto decompressor = LibJpegDecompressor{source};
  const auto outputImageSize = decompressor.outputImageSpecification().size;

  ASSERT_EQ(image::Size({128, 85}), outputImageSize);
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingSamplingMode_correctValueReturned1x1) {
  io::FileImageSource source{
      testdata::paths::jpeg::s16x16_cABCDF0_Q75_sampling444.normalized()};

  auto decompressor = LibJpegDecompressor{source};
  const auto chromaSamplingMode =
      decompressor.sourceImageSpecification().chromaSamplingMode;
  ASSERT_EQ(image::ChromaSamplingMode::S444, chromaSamplingMode);
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingSamplingMode_correctValueReturned2x2) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};

  auto decompressor = LibJpegDecompressor{source};
  const auto chromaSamplingMode =
      decompressor.sourceImageSpecification().chromaSamplingMode;
  ASSERT_EQ(image::ChromaSamplingMode::S420, chromaSamplingMode);
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingOutputSizeAndScaling_correctValueReturned) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto decompressor =
      LibJpegDecompressor{source, Configuration(), image::Ratio{2, 8}};
  const auto outputImageSize = decompressor.outputImageSpecification().size;

  ASSERT_EQ(image::Size({32, 22}), outputImageSize);
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingPixelSpecificationOfGrayscaleImage_thenGray8Returned) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_GRAYSCALE.normalized()};
  auto decompressor = LibJpegDecompressor{source};

  ASSERT_EQ(
      image::pixel::specifications::Gray,
      decompressor.sourceImageSpecification().pixelSpecification);
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingPixelSpecificationOfRgbImage_thenRgb888Returned) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto decompressor = LibJpegDecompressor{source};

  ASSERT_EQ(
      image::pixel::specifications::RGB,
      decompressor.outputImageSpecification().pixelSpecification);
}

//
// Reading scanlines GRAY_8
//

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingFromGrayscaleImage_thenSucceedsForEntireImage_white) {
  io::FileImageSource source{
      testdata::paths::jpeg::s16x16_WHITE_Q75_GRAYSCALE.normalized()};
  auto decompressor = LibJpegDecompressor{source};

  for (int i = 0; i < decompressor.outputImageSpecification().size.height;
       i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorGray(*scanline, {0xFF});
  }
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingFromGrayscaleImage_thenSucceedsForEntireImage_black) {
  io::FileImageSource source{
      testdata::paths::jpeg::s16x16_BLACK_Q75_GRAYSCALE.normalized()};
  auto decompressor = LibJpegDecompressor{source};

  for (int i = 0; i < decompressor.outputImageSpecification().size.height;
       i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorGray(*scanline, {0x00});
  }
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingTooManyScanlinesFromGrayscale_thenThrow) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_GRAYSCALE.normalized()};
  auto decompressor = LibJpegDecompressor{source};

  for (int i = 0; i < decompressor.outputImageSpecification().size.height;
       i++) {
    decompressor.readScanline();
  }

  ASSERT_THROW(decompressor.readScanline(), SpectrumException);
}

//
// Reading scanlines RGB_8
//

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingFromRgb888Image_thenSucceedsForEntireImage_white) {
  io::FileImageSource source{
      testdata::paths::jpeg::s16x16_WHITE_Q75.normalized()};
  auto decompressor = LibJpegDecompressor{source};

  for (int i = 0; i < decompressor.outputImageSpecification().size.height;
       i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {0xFF, 0xFF, 0xFF});
  }
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingFromRgb888Image_thenSucceedsForEntireImage_black) {
  io::FileImageSource source{
      testdata::paths::jpeg::s16x16_BLACK_Q75.normalized()};
  auto decompressor = LibJpegDecompressor{source};

  for (int i = 0; i < decompressor.outputImageSpecification().size.height;
       i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {0x00, 0x00, 0x00});
  }
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingFromRgb888Image_thenSucceedsForEntireImage_uniformColor) {
  io::FileImageSource source{
      testdata::paths::jpeg::s16x16_cABCDF0_Q75.normalized()};
  auto decompressor = LibJpegDecompressor{source};

  const auto outputImageSize = decompressor.outputImageSpecification().size;
  for (int i = 0; i < outputImageSize.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {0xAB, 0xCD, 0xF0});
  }
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingFromRgb888Image_withScaling_thenSucceedsForEntireImage_uniformColor) {
  io::FileImageSource source{
      testdata::paths::jpeg::s16x16_cABCDF0_Q75.normalized()};
  auto decompressor = LibJpegDecompressor{
      source, Configuration(), image::Ratio{2, 8}, folly::none};

  const auto outputImageSize = decompressor.outputImageSpecification().size;
  ASSERT_EQ(image::Size({4, 4}), outputImageSize);

  for (int i = 0; i < outputImageSize.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {0xAB, 0xCD, 0xF0});
  }
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingTooManyScanlinesFromRgb888_thenThrow) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_GRAYSCALE.normalized()};
  auto decompressor = LibJpegDecompressor{source};

  const auto outputImageSize = decompressor.outputImageSpecification().size;
  for (int i = 0; i < outputImageSize.height; i++) {
    decompressor.readScanline();
  }

  ASSERT_THROW(decompressor.readScanline(), SpectrumException);
}

//
// Special images and JPEG variations
//

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenReadingJpegWithMissingEoiAndGarbage_thenDontThrow) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE_MALFORMED_SAMSUNG_STYLE
          .normalized()};
  auto decompressor = LibJpegDecompressor{source};

  const auto outputImageSize = decompressor.outputImageSpecification().size;
  for (int i = 0; i < outputImageSize.height; i++) {
    const auto scanline = decompressor.readScanline();
  }
}

//
// Overriding pixel type
//

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenOverridingToGray8_thenMatchesGetInputPixelSpecificationAndReads) {
  io::FileImageSource source{
      testdata::paths::jpeg::s16x16_cABCDF0_Q75.normalized()};
  auto decompressor = LibJpegDecompressor{source,
                                          Configuration(),
                                          LIBJPEG_SCALE_DEFAULT,
                                          image::pixel::specifications::Gray};

  ASSERT_EQ(
      image::pixel::specifications::Gray,
      decompressor.outputImageSpecification().pixelSpecification);

  for (int i = 0; i < 16; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorGray(*scanline, {0xC7});
  }
}

TEST(
    plugins_jpeg_LibJpegDecompressor,
    whenOverridingToRgb8_thenMatchesGetInputPixelSpecificationAndReads) {
  io::FileImageSource source{
      testdata::paths::jpeg::s16x16_WHITE_Q75_GRAYSCALE.normalized()};
  auto decompressor = LibJpegDecompressor{source,
                                          Configuration(),
                                          LIBJPEG_SCALE_DEFAULT,
                                          image::pixel::specifications::RGB};

  ASSERT_EQ(
      image::pixel::specifications::RGB,
      decompressor.outputImageSpecification().pixelSpecification);

  for (int i = 0; i < 16; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {0xFF, 0xFF, 0xFF});
  }
}

} // namespace test
} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
