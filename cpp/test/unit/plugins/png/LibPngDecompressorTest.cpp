// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/plugins/png/LibPngDecompressor.h>
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
namespace png {
namespace test {

TEST(plugins_png_LibPngDecompressor, whenConstructed_nothingObservableHappens) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_1B.normalized()};
  auto decompressor = LibPngDecompressor{source};
}

//
// Header information
//

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_gray1bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_1B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::Gray,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_gray2bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_2B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::Gray,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_gray4bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_4B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::Gray,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_gray8bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_8B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::Gray,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_graya8bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_A_8B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::GrayA,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_graya16bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_A_16B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::GrayA,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_gray_trns_8bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_TRNS_8B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::GrayA,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_gray_trns_16bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_TRNS_16B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::GrayA,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_rgb4bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_cABCDEF_RGB_4B.normalized()};
  auto decompressor = LibPngDecompressor{source};

  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::RGB,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_rgb8bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_cABCDEF_RGB_8B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::RGB,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_rgb8bit_thenPixelSpecificationAndDimensionCorrect_128x85) {
  io::FileImageSource source{testdata::paths::png::s128x85_RGB.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::RGB,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{128, 85}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_rgb16bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_cABCDEF_RGB_16B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::RGB,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_argb8bit_thenPixelSpecificationAndDimensionCorrect) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_a50_cABCDEF_ARGB_8B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  ASSERT_EQ(
      image::pixel::specifications::ARGB,
      inputImageSpecification.pixelSpecification);
  ASSERT_EQ((image::Size{16, 16}), inputImageSpecification.size);
}

//
// Reading scanlines
//

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_gray8bit_thenSuccessForEntireImage_black) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_BLACK_GRAY_8B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  for (int i = 0; i < inputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorGray(*scanline, {0x00});
  }
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_gray4bit_thenSuccessForEntireImage_white) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_WHITE_GRAY_4B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  for (int i = 0; i < inputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorGray(*scanline, {0xFF});
  }
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_rgb8bit_thenSuccessForEntireImage_cABCDEF) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_cABCDEF_RGB_8B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  for (int i = 0; i < inputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {0xAB, 0xCD, 0xEF});
  }
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_rgb8bit_thenSuccessForEntireImage_cABCDEF_interlaced) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_cABCDEF_RGB_8B_INTERLACED.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  for (int i = 0; i < inputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {{0xAB, 0xCD, 0xEF}});
  }
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_rgb16bit_thenSuccessForEntireImage_cABCDEF) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_cABCDEF_RGB_8B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  for (int i = 0; i < inputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorRgb(*scanline, {{0xAB, 0xCD, 0xEF}});
  }
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_rgb8bit_thenSuccessForEntireImage_128x85) {
  io::FileImageSource source{testdata::paths::png::s128x85_RGB.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  for (int i = 0; i < inputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
  }
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_rgb8bit_thenSuccessForEntireImage_128x85_interlaced) {
  io::FileImageSource source{
      testdata::paths::png::s128x85_RGB_INTERLACED.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  for (int i = 0; i < inputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
  }
}

TEST(
    plugins_png_LibPngDecompressor,
    whenReading_argb8bit_thenSuccessForEntireImage_a50_cABCDEF) {
  io::FileImageSource source{
      testdata::paths::png::s16x16_a50_cABCDEF_ARGB_8B.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  for (int i = 0; i < inputImageSpecification.size.height; i++) {
    const auto scanline = decompressor.readScanline();
    image::testutils::assertScanlineIsColorArgb(
        *scanline, {{0x80, 0xAB, 0xCD, 0xEF}});
  }
}

//
// Error handling
//

TEST(
    plugins_png_LibPngDecompressor,
    whenRead22ing_rgb8bit_truncated_thenThrows) {
  io::FileImageSource source{
      testdata::paths::png::s128x85_RGB_TRUNCATED.normalized()};
  auto decompressor = LibPngDecompressor{source};
  const auto inputImageSpecification = decompressor.sourceImageSpecification();

  try {
    for (int i = 0; i < inputImageSpecification.size.height; i++) {
      decompressor.readScanline();
    }
    FAIL();
  } catch (const SpectrumException& e) {
    ASSERT_TRUE(std::strlen(e.what()) > 0);
  } catch (const std::exception& e) {
    FAIL();
  }
}

} // namespace test
} // namespace png
} // namespace plugins
} // namespace spectrum
} // namespace facebook
