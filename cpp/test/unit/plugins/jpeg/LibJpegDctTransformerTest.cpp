// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegTestHelper.h"

#include <spectrum/Configuration.h>
#include <spectrum/io/FileImageSource.h>
#include <spectrum/plugins/jpeg/LibJpegDctTransformer.h>
#include <spectrum/requirements/CropAbsoluteToOrigin.h>
// #include "../TestData.h"
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

TEST(
    plugins_jpeg_LibJpegDctTransformer,
    whenConstructed_thenNothingObservableHappens) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};

  LibJpegDctTransformer dctTransformer(source, sink);
}

//
// Base usage
//

TEST(
    plugins_jpeg_LibJpegDctTransformer,
    whenNoTransformation_thenOutputSameSize) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.applyAndFinish();

  ASSERT_TRUE(testutils::assertOutputValidJpeg(sink, image::Size{128, 85}));
  ASSERT_EQ((image::Size{128, 85}), dctTransformer.getOutputSize());
}

TEST(plugins_jpeg_LibJpegDctTransformer, whenApplyingTwice_thenThrow) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.applyAndFinish();
  ASSERT_THROW(dctTransformer.applyAndFinish(), SpectrumException);
}

//
// Rotation
//

TEST(plugins_jpeg_LibJpegDctTransformer, whenRotation90_thenOutputSizeChanged) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.setRotateRequirement(requirements::Rotate{.degrees = 90});
  dctTransformer.applyAndFinish();

  // size change to MCU multiplies!
  ASSERT_TRUE(testutils::assertOutputValidJpeg(sink, image::Size{80, 128}));
  ASSERT_EQ((image::Size{80, 128}), dctTransformer.getOutputSize());
}

TEST(
    plugins_jpeg_LibJpegDctTransformer,
    whenRotation180_thenOutputSizeChanged) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.setRotateRequirement(requirements::Rotate{.degrees = 180});
  dctTransformer.applyAndFinish();

  ASSERT_TRUE(testutils::assertOutputValidJpeg(sink, image::Size{128, 80}));
  ASSERT_EQ((image::Size{128, 80}), dctTransformer.getOutputSize());
}

TEST(plugins_jpeg_LibJpegDctTransformer, whenRotationAfterApply_thenThrow) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.applyAndFinish();
  const auto rotateRequirement = requirements::Rotate{.degrees = 90};
  ASSERT_THROW(
      dctTransformer.setRotateRequirement(rotateRequirement),
      SpectrumException);
}

//
// Cropping
//

TEST(
    plugins_jpeg_LibJpegDctTransformer,
    whenCroppingAbsoluteOnMcuBoundsWithMcuSize_thenMatchesSize) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};
  const auto cropRequirement = requirements::CropAbsoluteToOrigin(
      {.top = 16, .left = 16, .bottom = 64, .right = 64}, false);

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.setCropRequirement(cropRequirement);
  dctTransformer.applyAndFinish();

  ASSERT_TRUE(
      testutils::assertOutputValidJpeg(sink, image::Size{64 - 16, 64 - 16}));
  ASSERT_EQ((image::Size{64 - 16, 64 - 16}), dctTransformer.getOutputSize());
}

TEST(
    plugins_jpeg_LibJpegDctTransformer,
    whenCroppingAbsoluteOnMcuBoundsWithNonMcuSize_thenSizeMatch) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};
  const auto cropRequirement = requirements::CropAbsoluteToOrigin(
      {.top = 16, .left = 16, .bottom = 63, .right = 67}, false);

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.setCropRequirement(cropRequirement);
  dctTransformer.applyAndFinish();

  ASSERT_TRUE(
      testutils::assertOutputValidJpeg(sink, image::Size{67 - 16, 63 - 16}));
  ASSERT_EQ((image::Size{67 - 16, 63 - 16}), dctTransformer.getOutputSize());
}

TEST(
    plugins_jpeg_LibJpegDctTransformer,
    whenCroppingAbsoluteNotOnMcuBoundsWithNonMcuSize_thenSizeIncreased) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};

  const auto cropRequirement = requirements::CropAbsoluteToOrigin(
      {.top = 17, .left = 21, .bottom = 63, .right = 67}, false);

  LibJpegDctTransformer dctTransformer(source, sink);

  dctTransformer.setCropRequirement(cropRequirement);
  dctTransformer.applyAndFinish();

  // top-left corner gets moved until hits MCU boundary
  ASSERT_TRUE(
      testutils::assertOutputValidJpeg(sink, image::Size{67 - 16, 63 - 16}));
  ASSERT_EQ((image::Size{67 - 16, 63 - 16}), dctTransformer.getOutputSize());
}

TEST(plugins_jpeg_LibJpegDctTransformer, whenCroppingAfterApply_thenThrow) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};
  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.applyAndFinish();

  ASSERT_THROW(
      dctTransformer.setCropRequirement(folly::none), SpectrumException);
}

//
// Rotating and cropping
//

TEST(
    plugins_jpeg_LibJpegDctTransformer,
    whenRotating90AndCropping_thenMatchesExpectations) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};
  const auto cropRequirement = requirements::CropAbsoluteToOrigin(
      {.top = 16, .left = 16, .bottom = 64, .right = 32}, false);

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.setRotateRequirement(requirements::Rotate{.degrees = 90});
  dctTransformer.setCropRequirement(cropRequirement);
  dctTransformer.applyAndFinish();

  // top-left moves as not on MCU boundary after rotate
  ASSERT_TRUE(testutils::assertOutputValidJpeg(sink, image::Size{53, 16}));
  ASSERT_EQ((image::Size{53, 16}), dctTransformer.getOutputSize());
}

TEST(
    plugins_jpeg_LibJpegDctTransformer,
    whenRotating180AndCropping_thenMatchesExpectations) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};
  const auto cropRequirement = requirements::CropAbsoluteToOrigin(
      {.top = 16, .left = 16, .bottom = 64, .right = 32}, false);

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.setRotateRequirement(requirements::Rotate{.degrees = 180});
  dctTransformer.setCropRequirement(cropRequirement);
  dctTransformer.applyAndFinish();

  // top-left moves as not on MCU boundary after rotate
  ASSERT_TRUE(testutils::assertOutputValidJpeg(sink, image::Size{16, 53}));
  ASSERT_EQ((image::Size{16, 53}), dctTransformer.getOutputSize());
}

TEST(
    plugins_jpeg_LibJpegDctTransformer,
    whenRotating270AndCropping_thenMatchesExpectations) {
  io::FileImageSource source{
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized()};
  auto sink = io::testutils::FakeImageSink{};
  const auto cropRequirement = requirements::CropAbsoluteToOrigin(
      {.top = 16, .left = 16, .bottom = 64, .right = 32}, false);

  LibJpegDctTransformer dctTransformer(source, sink);
  dctTransformer.setRotateRequirement(requirements::Rotate{.degrees = 270});
  dctTransformer.setCropRequirement(cropRequirement);
  dctTransformer.applyAndFinish();

  // on MCU boundary as height is multiple of MCU
  ASSERT_TRUE(
      testutils::assertOutputValidJpeg(sink, image::Size{64 - 16, 32 - 16}));
  ASSERT_EQ((image::Size{64 - 16, 32 - 16}), dctTransformer.getOutputSize());
}

} // namespace test
} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
