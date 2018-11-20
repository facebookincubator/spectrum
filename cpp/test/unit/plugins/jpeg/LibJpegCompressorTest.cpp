// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegTestHelper.h"

#include <spectrum/Configuration.h>
#include <spectrum/plugins/jpeg/LibJpegCompressor.h>
#include <spectrum/plugins/jpeg/LibJpegDecompressor.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <cstddef>
#include <memory>
#include <string>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {
namespace test {

::testing::AssertionResult writeScanlinesAndAssertOutputForSanity(
    LibJpegCompressor& compressor,
    io::testutils::FakeImageSink& sink,
    image::Size imageSize);

namespace {

LibJpegCompressor makeCompressor(
    codecs::testutils::CompressorOptions&& options) {
  return LibJpegCompressor(codecs::CompressorOptions{
      .sink = options.sink,
      .imageSpecification =
          image::Specification{
              .size = options.imageSize.value_or(
                  image::testutils::DefaultImageSize),
              .format = image::formats::Jpeg,
              .pixelSpecification = options.pixelSpecification.value_or(
                  image::pixel::specifications::RGB),
              .metadata = options.metadata.value_or(image::Metadata{}),
          },
      .encodeRequirement =
          options.encodeRequirement.value_or(requirements::Encode{
              .format = image::formats::Jpeg,
              .mode = requirements::Encode::Mode::Lossy,
              .quality = 80,
          }),
      .configuration = options.configuration,
  });
}

void assertValidScanlinesUsingConfiguration(
    const Configuration& configuration) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor =
      makeCompressor({.sink = sink, .configuration = configuration});

  ASSERT_TRUE(writeScanlinesAndAssertOutputForSanity(
      compressor, sink, image::testutils::DefaultImageSize));
}
} // namespace

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenConstructed_thenNothingObservableHappens) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({.sink = sink});
  ASSERT_EQ("", sink.stringContent());
}

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenConstructedWithLosslessEncodeRequirement_thenThrows) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressorFactory = [&sink] {
    return makeCompressor({
        .sink = sink,
        .encodeRequirement =
            requirements::Encode{
                .format = image::formats::Jpeg,
                .mode = requirements::Encode::Mode::Lossless,
            },
    });
  };

  ASSERT_THROW(compressorFactory(), SpectrumException);
}

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenConstructed_thenConfigurationPropagatedToImageSink) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({.sink = sink});

  ASSERT_EQ("", sink.stringContent());
  ASSERT_EQ(image::testutils::DefaultImageSize, sink.size());
  ASSERT_EQ(image::pixel::specifications::RGB, sink.getPixelSpecification());
}

//
// Configuration
//

TEST(plugins_jpeg_LibJpegCompressor, whenTurningTrellisOff_outputImageValid) {
  Configuration configuration;
  configuration.jpeg.useTrellis(false);
  assertValidScanlinesUsingConfiguration(configuration);
}

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenTurningOptimizeScansOn_outputImageValid) {
  Configuration configuration;
  configuration.jpeg.useOptimizeScan(true);
  assertValidScanlinesUsingConfiguration(configuration);
}

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenTurningCompatibleDcScanOff_outputImageValid) {
  Configuration configuration;
  configuration.jpeg.useCompatibleDcScanOpt(false);
  assertValidScanlinesUsingConfiguration(configuration);
}

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenTurning444SamplingOn_outputImageValid) {
  Configuration configuration;
  configuration.general.chromaSamplingModeOverride(
      Configuration::General::ChromaSamplingModeOverride::S444);
  assertValidScanlinesUsingConfiguration(configuration);
}

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenTurningPsnrQuantTableOn_outputImageValid) {
  Configuration configuration;
  configuration.jpeg.usePsnrQuantTable(true);
  assertValidScanlinesUsingConfiguration(configuration);
}

//
// Write
//

TEST(plugins_jpeg_LibJpegCompressor, whenWritingGray8ButImageRgb_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor(
      {.sink = sink,
       .imageSize = image::Size{1, 5},
       .pixelSpecification = image::pixel::specifications::Gray});
  auto scanline = image::testutils::makeScanlineRgb({{128, 128, 128}});
  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(plugins_jpeg_LibJpegCompressor, whenWritingRgb888ButImageGray_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor =
      makeCompressor({.sink = sink, .imageSize = image::Size{1, 5}});

  auto scanline = image::testutils::makeScanlineGray({{128}});
  ASSERT_THROW(
      compressor.writeScanline(std::move(scanline)), SpectrumException);
}

TEST(
    plugins_jpeg_LibJpegCompressor,
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

TEST(plugins_jpeg_LibJpegCompressor, whenWritingTooManyScanlines_thenThrow) {
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

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenWritingAllLines_thenOutputImageShowsJpegPrefixAndSuffix_tiny) {
  auto sink = io::testutils::FakeImageSink{};
  const image::Size imageSize{1, 1};
  auto compressor = makeCompressor({.sink = sink, .imageSize = imageSize});

  ASSERT_TRUE(
      writeScanlinesAndAssertOutputForSanity(compressor, sink, imageSize));
}

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenWritingAllLines_thenOutputImageShowsJpegPrefixAndSuffix_medium) {
  auto sink = io::testutils::FakeImageSink{};
  const image::Size imageSize{63, 41};
  auto compressor = makeCompressor({.sink = sink, .imageSize = imageSize});

  ASSERT_TRUE(
      writeScanlinesAndAssertOutputForSanity(compressor, sink, imageSize));
}

TEST(
    plugins_jpeg_LibJpegCompressor,
    whenWritingAllLines_thenOutputImageShowsJpegPrefixAndSuffix_large) {
  auto sink = io::testutils::FakeImageSink{};
  const image::Size imageSize{
      512, 512}; // large enough to cause multiple imageSinkEmptyBuffer calls
  auto compressor = makeCompressor({.sink = sink, .imageSize = imageSize});

  ASSERT_TRUE(
      writeScanlinesAndAssertOutputForSanity(compressor, sink, imageSize));
}

TEST(plugins_jpeg_LibJpegCompressor, whenQualityLessThanMin_thenSanitizeToMin) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor =
      makeCompressor({.sink = sink,
                      .encodeRequirement = requirements::Encode{
                          .format = image::formats::Jpeg, .quality = 2}});

  ASSERT_EQ(LibJpegCompressor::QualityMin, compressor.quality);
}

TEST(plugins_jpeg_LibJpegCompressor, whenQualityMoreThanMax_thenSanitizeToMax) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor =
      makeCompressor({.sink = sink,
                      .encodeRequirement = requirements::Encode{
                          .format = image::formats::Jpeg, .quality = 101}});

  ASSERT_EQ(LibJpegCompressor::QualityMax, compressor.quality);
}

TEST(plugins_jpeg_LibJpegCompressor, whenQualityIsWithinBounds_thenSame) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor =
      makeCompressor({.sink = sink,
                      .encodeRequirement = requirements::Encode{
                          .format = image::formats::Jpeg, .quality = 42}});

  ASSERT_EQ(42, compressor.quality);
}

::testing::AssertionResult writeScanlinesAndAssertOutputForSanity(
    LibJpegCompressor& compressor,
    io::testutils::FakeImageSink& sink,
    image::Size imageSize) {
  codecs::testutils::writeRgbData(compressor, imageSize);
  return testutils::assertOutputValidJpeg(sink, imageSize);
}

} // namespace test
} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
