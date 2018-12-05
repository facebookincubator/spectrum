// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/plugins/png/LibPngCompressor.h>
#include <spectrum/Configuration.h>
#include <spectrum/plugins/png/LibPngDecompressor.h>
#include <spectrum/testutils/TestUtils.h>

#include <signal.h>
#include <array>
#include <cstddef>
#include <memory>
#include <string>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace png {
namespace test {

static ::testing::AssertionResult writeScanlinesAndAssertOutputForSanity(
    LibPngCompressor& compressor,
    io::testutils::FakeImageSink& sink,
    const image::Size& imageSize,
    const Configuration& configuration);

namespace {

LibPngCompressor makeCompressor(
    codecs::testutils::CompressorOptions&& options) {
  return LibPngCompressor(codecs::CompressorOptions{
      .sink = options.sink,
      .imageSpecification =
          image::Specification{
              .size = options.imageSize.value_or(
                  image::testutils::DefaultImageSize),
              .format = image::formats::Png,
              .pixelSpecification = options.pixelSpecification.value_or(
                  image::pixel::specifications::RGB),
              .metadata = options.metadata.value_or(image::Metadata{}),
          },
      .encodeRequirement = options.encodeRequirement,
      .configuration = options.configuration,
  });
}

static void assertValidScanlinesUsingConfiguration(
    const Configuration& configuration) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor =
      makeCompressor({.sink = sink, .configuration = configuration});

  ASSERT_TRUE(writeScanlinesAndAssertOutputForSanity(
      compressor, sink, image::testutils::DefaultImageSize, configuration));
}
} // namespace

TEST(
    plugins_png_LibPngCompressor,
    whenConstructed_thenNothingObservableHappens) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({.sink = sink});
  ASSERT_EQ("", sink.stringContent());
}

TEST(
    plugins_png_LibPngCompressor,
    whenConstructed_thenConfigurationPropagatedToImageSink) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({.sink = sink});

  ASSERT_EQ("", sink.stringContent());
  ASSERT_EQ(image::testutils::DefaultImageSize, sink.size());
  ASSERT_EQ(image::pixel::specifications::RGB, sink.getPixelSpecification());
}

//
// Options
//

TEST(plugins_png_LibPngCompressor, whenTurningInterlacingOn_outputImageValid) {
  Configuration configuration;
  configuration.png.useInterlacing(true);
  assertValidScanlinesUsingConfiguration(configuration);
}

TEST(plugins_png_LibPngCompressor, whenTurningInterlacingOff_outputImageValid) {
  Configuration configuration;
  configuration.png.useInterlacing(false);
  assertValidScanlinesUsingConfiguration(configuration);
}

//
// Write
//

TEST(
    plugins_png_LibPngCompressor,
    whenWritingImageWithZeroDimension_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor(
      {.sink = sink,
       .imageSize = image::Size{1, 0},
       .pixelSpecification = image::pixel::specifications::Gray});

  auto scanline = image::testutils::makeScanlineGray({{128}});

  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(plugins_png_LibPngCompressor, whenWritingGray8ButImageRgb_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor(
      {.sink = sink,
       .imageSize = image::Size{1, 5},
       .pixelSpecification = image::pixel::specifications::Gray});
  auto scanline = image::testutils::makeScanlineRgb({{128, 128, 128}});

  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(plugins_png_LibPngCompressor, whenWritingRgb888ButImageGray_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor =
      makeCompressor({.sink = sink, .imageSize = image::Size{1, 5}});
  auto scanline = image::testutils::makeScanlineGray({{128}});

  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(
    plugins_png_LibPngCompressor,
    whenWritingScanlineWithWrongWidth_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor(
      {.sink = sink,
       .imageSize = image::Size{1, 5},
       .pixelSpecification = image::pixel::specifications::Gray});
  auto scanline = image::testutils::makeScanlineGray({{128}, {128}});

  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(plugins_png_LibPngCompressor, whenWritingTooManyScanlines_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor(
      {.sink = sink,
       .imageSize = image::Size{2, 2},
       .pixelSpecification = image::pixel::specifications::Gray});

  compressor.writeScanline(image::testutils::makeScanlineGray({{128}, {128}}));
  compressor.writeScanline(image::testutils::makeScanlineGray({{128}, {128}}));
  ASSERT_THROW(
      compressor.writeScanline(
          image::testutils::makeScanlineGray({{128}, {128}})),
      SpectrumException);
}

static ::testing::AssertionResult writeScanlinesAndAssertOutputForSanity(
    LibPngCompressor& compressor,
    io::testutils::FakeImageSink& sink,
    const image::Size& imageSize,
    const Configuration& configuration) {
  codecs::testutils::writeRgbData(compressor, imageSize);

  // inspect output
  const auto output = sink.stringContent();
  if (output.size() == 0) {
    return ::testing::AssertionFailure() << "output is empty";
  }

  // SOI
  if ('\x89' != output[0]) {
    return ::testing::AssertionFailure()
        << "output header does not start with 0x89";
  }

  // 'PNG' string
  if ('P' != output[1] || 'N' != output[2] || 'G' != output[3]) {
    return ::testing::AssertionFailure()
        << "output header does not contain with \"PNG\" string (0x50 0x4E 0x47)";
  }

  // DOS-style line ending
  if ('\x0D' != output[4] || '\x0A' != output[5]) {
    return ::testing::AssertionFailure()
        << "output header does not contain DOS-style line heading (0x0D 0x0A)";
  }

  // DOS EOF & Line feed
  if ('\x1A' != output[6] || '\x0A' != output[7]) {
    return ::testing::AssertionFailure()
        << "output header does not contain DOS EOF & Line Feed (0x1A 0x0A)";
  }

  // Chunk Type is IHDR
  if ('I' != output[12] || 'H' != output[13] || 'D' != output[14] ||
      'R' != output[15]) {
    return ::testing::AssertionFailure()
        << "output does not contain IHDR Chunk";
  }

  // IHDR interlace method field
  auto useInterlacing = configuration.png.useInterlacing();
  if (useInterlacing && '\x01' != output[28]) {
    return ::testing::AssertionFailure()
        << "output IHDR's interlace method is not set to Adam7 interlacing (0x01)";
  } else if (!useInterlacing && '\x00' != output[28]) {
    return ::testing::AssertionFailure()
        << "output IHDR's interlace method is not set to No interlacing (0x01)";
  }

  // re-read into png decompressor and validate header information
  auto source = io::testutils::makeVectorImageSource(output);
  auto decompressor = LibPngDecompressor{source};

  const auto newImageSize = decompressor.sourceImageSpecification().size;
  if (newImageSize.width != imageSize.width ||
      newImageSize.height != imageSize.height) {
    return ::testing::AssertionFailure()
        << "reading the generated output lead to different size";
  }

  return ::testing::AssertionSuccess();
}
} // namespace test
} // namespace png
} // namespace plugins
} // namespace spectrum
} // namespace facebook
