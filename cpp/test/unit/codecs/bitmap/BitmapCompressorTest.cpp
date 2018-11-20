// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/codecs/bitmap/BitmapCompressor.h>
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
namespace {

BitmapCompressor makeCompressor(
    codecs::testutils::CompressorOptions&& options) {
  return BitmapCompressor{codecs::CompressorOptions{
      .sink = options.sink,
      .imageSpecification =
          image::Specification{
              .format = image::formats::Bitmap,
              .size = options.imageSize.value_or(
                  image::testutils::DefaultImageSize),
              .pixelSpecification = options.pixelSpecification.value_or(
                  image::pixel::specifications::RGB),
              .metadata = options.metadata.value_or(image::Metadata{}),
          },
      .encodeRequirement = options.encodeRequirement,
  }};
}
} // namespace

TEST(
    codecs_bitmap_BitmapCompressor,
    whenConstructed_thenNothingObservableHappens) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
  });

  ASSERT_EQ("", sink.stringContent());
}

TEST(
    codecs_bitmap_BitmapCompressor,
    whenConstructed_thenConfigurationPropagatedToImageSink) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
  });

  ASSERT_EQ("", sink.stringContent());
  ASSERT_EQ(image::testutils::DefaultImageSize, sink.size());
  ASSERT_EQ(image::pixel::specifications::RGB, sink.getPixelSpecification());
}

//
// Write
//

TEST(codecs_bitmap_BitmapCompressor, whenWritingGray8_thenOutputMatches) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
      .imageSize = image::Size{2, 2},
      .pixelSpecification = image::pixel::specifications::Gray,
  });

  compressor.writeScanline(
      image::testutils::makeScanlineGray({{0x61}, {0x62}}));
  compressor.writeScanline(
      image::testutils::makeScanlineGray({{0x63}, {0x64}}));
  ASSERT_EQ("\x61\x62\x63\x64", sink.stringContent());
}

TEST(codecs_bitmap_BitmapCompressor, whenWritingRgb888_thenOutputMatches) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
      .imageSize = image::Size{2, 2},
      .pixelSpecification = image::pixel::specifications::RGB,
  });

  compressor.writeScanline(image::testutils::makeScanlineRgb(
      {{0x10, 0x11, 0x12}, {0x20, 0x21, 0x22}}));
  compressor.writeScanline(image::testutils::makeScanlineRgb(
      {{0x30, 0x31, 0x32}, {0x40, 0x41, 0x42}}));
  ASSERT_EQ(
      "\x10\x11\x12\x20\x21\x22\x30\x31\x32\x40\x41\x42", sink.stringContent());
}

TEST(codecs_bitmap_BitmapCompressor, whenWritingArgb8888_thenOutputMatches) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
      .imageSize = image::Size{2, 2},
      .pixelSpecification = image::pixel::specifications::ARGB,
  });

  compressor.writeScanline(image::testutils::makeScanlineArgb(
      {{0x10, 0x11, 0x12, 0x13}, {0x20, 0x21, 0x22, 0x23}}));
  compressor.writeScanline(image::testutils::makeScanlineArgb(
      {{0x30, 0x31, 0x32, 0x33}, {0x40, 0x41, 0x42, 0x43}}));
  ASSERT_EQ(
      "\x10\x11\x12\x13\x20\x21\x22\x23\x30\x31\x32\x33\x40\x41\x42\x43",
      sink.stringContent());
}

TEST(codecs_bitmap_BitmapCompressor, whenWritingRgba8888_thenOutputMatches) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
      .imageSize = image::Size{2, 2},
      .pixelSpecification = image::pixel::specifications::RGBA,
  });

  compressor.writeScanline(image::testutils::makeScanlineRgba(
      {{0x10, 0x11, 0x12, 0x13}, {0x20, 0x21, 0x22, 0x23}}));
  compressor.writeScanline(image::testutils::makeScanlineRgba(
      {{0x30, 0x31, 0x32, 0x33}, {0x40, 0x41, 0x42, 0x43}}));
  ASSERT_EQ(
      "\x10\x11\x12\x13\x20\x21\x22\x23\x30\x31\x32\x33\x40\x41\x42\x43",
      sink.stringContent());
}

//
// Write error handling
//

TEST(
    codecs_bitmap_BitmapCompressor,
    whenWritingGray8_butImageConfigurationDifferent_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
      .imageSize = image::Size{1, 1},
      .pixelSpecification = image::pixel::specifications::RGB,
  });

  auto scanline = image::testutils::makeScanlineGray({{128}});
  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(
    codecs_bitmap_BitmapCompressor,
    whenWritingRgb888_butImageConfigurationDifferent_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
      .imageSize = image::Size{1, 1},
      .pixelSpecification = image::pixel::specifications::Gray,
  });

  auto scanline = image::testutils::makeScanlineRgb({{128, 128, 128}});
  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(
    codecs_bitmap_BitmapCompressor,
    whenWritingArgb888_butImageConfigurationDifferent_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
      .imageSize = image::Size{1, 1},
      .pixelSpecification = image::pixel::specifications::RGB,
  });

  auto scanline = image::testutils::makeScanlineArgb({{128, 128, 128, 128}});
  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(
    codecs_bitmap_BitmapCompressor,
    whenWritingLongerScanlinesThanExpected_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
      .imageSize = image::Size{2, 1},
      .pixelSpecification = image::pixel::specifications::Gray,
  });

  auto scanline = image::testutils::makeScanlineGray({{128}, {128}, {128}});
  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(
    codecs_bitmap_BitmapCompressor,
    whenWritingMoreScanlinesThenExpected_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({
      .sink = sink,
      .imageSize = image::Size{1, 1},
      .pixelSpecification = image::pixel::specifications::Gray,
  });

  auto scanline1 = image::testutils::makeScanlineGray({{128}});
  compressor.writeScanline(std::move(scanline1));

  auto scanline2 = image::testutils::makeScanlineGray({{128}});
  ASSERT_ANY_THROW(compressor.writeScanline(std::move(scanline2)));
}

} // namespace test
} // namespace bitmap
} // namespace codecs
} // namespace spectrum
} // namespace facebook
