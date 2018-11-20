// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/codecs/bitmap/BitmapDecompressor.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <cstddef>
#include <memory>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace bitmap {
namespace test {

//
// Read
//

/** A 4 pixel long bitmap with 4 components */
static const std::string sampleBitmap =
    "\x10\x11\x12\x13\x20\x21\x22\x23\x30\x31\x32\x33\x40\x41\x42\x43";

TEST(plugins_bitmap_BitmapDecompressor, whenReadingRgba_thenScanlinesMatch) {
  auto source = io::testutils::makeVectorBitmapImageSource(
      sampleBitmap,
      image::Specification{
          .size = image::Size{2, 2},
          .format = image::formats::Bitmap,
          .pixelSpecification = image::pixel::specifications::RGBA,
      });
  auto decompressor = BitmapDecompressor{source};

  image::testutils::assertScanlineRgba(
      {{0x10, 0x11, 0x12, 0x13}, {0x20, 0x21, 0x22, 0x23}},
      decompressor.readScanline().get());

  image::testutils::assertScanlineRgba(
      {{0x30, 0x31, 0x32, 0x33}, {0x40, 0x41, 0x42, 0x43}},
      decompressor.readScanline().get());
}

TEST(codecs_bitmap_BitmapDecompressor, whenReadingArgb_thenScanlinesMatch) {
  auto source = io::testutils::makeVectorBitmapImageSource(
      sampleBitmap,
      image::Specification{
          .size = image::Size{2, 2},
          .format = image::formats::Bitmap,
          .pixelSpecification = image::pixel::specifications::ARGB,
      });
  auto decompressor = BitmapDecompressor{source};

  image::testutils::assertScanlineArgb(
      {{0x13, 0x10, 0x11, 0x12}, {0x13, 0x10, 0x11, 0x12}},
      decompressor.readScanline().get());

  image::testutils::assertScanlineArgb(
      {{0x33, 0x30, 0x31, 0x32}, {0x43, 0x40, 0x41, 0x42}},
      decompressor.readScanline().get());
}

TEST(codecs_bitmap_BitmapDecompressor, whenReadingRgb_thenScanlinesMatch) {
  auto source = io::testutils::makeVectorBitmapImageSource(
      sampleBitmap,
      image::Specification{
          .size = image::Size{2, 2},
          .format = image::formats::Bitmap,
          .pixelSpecification = image::pixel::specifications::RGB,
      });
  auto decompressor = BitmapDecompressor{source};

  image::testutils::assertScanlineRgb(
      {{0x11, 0x12, 0x13}, {0x21, 0x22, 0x23}},
      decompressor.readScanline().get());

  image::testutils::assertScanlineRgb(
      {{0x31, 0x32, 0x33}, {0x41, 0x42, 0x43}},
      decompressor.readScanline().get());
}

TEST(codecs_bitmap_BitmapDecompressor, whenReadingGray_thenScanlinesMatch) {
  const auto sampleGrayBitmap = "\x10\x11\x12\x13";
  auto source = io::testutils::makeVectorBitmapImageSource(
      sampleGrayBitmap,
      image::Specification{
          .size = image::Size{2, 2},
          .format = image::formats::Bitmap,
          .pixelSpecification = image::pixel::specifications::Gray,
      });
  auto decompressor = BitmapDecompressor{source};

  image::testutils::assertScanlineGray(
      {{0x10}, {0x11}}, decompressor.readScanline().get());

  image::testutils::assertScanlineGray(
      {{0x12}, {0x13}}, decompressor.readScanline().get());
}

TEST(codecs_bitmap_BitmapDecompressor, whenReadingTooMuch_thenThrow) {
  auto source = io::testutils::makeVectorBitmapImageSource(
      sampleBitmap,
      image::Specification{
          .size = image::Size{2, 2},
          .format = image::formats::Bitmap,
          .pixelSpecification = image::pixel::specifications::RGBA,
      });
  auto decompressor = BitmapDecompressor{source};

  decompressor.readScanline();
  decompressor.readScanline();
  ASSERT_THROW(decompressor.readScanline(), SpectrumException);
}

} // namespace test
} // namespace bitmap
} // namespace codecs
} // namespace spectrum
} // namespace facebook
