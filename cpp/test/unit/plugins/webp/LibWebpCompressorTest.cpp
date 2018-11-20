// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/plugins/webp/LibWebpCompressor.h>
#include <spectrum/testutils/TestUtils.h>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace webp {
namespace test {
namespace {

LibWebpCompressor makeCompressor(
    codecs::testutils::CompressorOptions&& options) {
  return LibWebpCompressor(codecs::CompressorOptions{
      .sink = options.sink,
      .imageSpecification =
          image::Specification{
              .size = options.imageSize.value_or(
                  image::testutils::DefaultImageSize),
              .format = image::formats::Webp,
              .pixelSpecification = options.pixelSpecification.value_or(
                  image::pixel::specifications::RGB),
              .metadata = options.metadata.value_or(image::Metadata{}),
          },
      .encodeRequirement =
          options.encodeRequirement.value_or(requirements::Encode{
              .format = image::formats::Webp,
          }),
  });
}
} // namespace

TEST(
    plugins_webp_LibWebpCompressor,
    whenConstructed_thenNothingObservableHappens) {
  auto sink = io::testutils::FakeImageSink{};

  ASSERT_NO_THROW(makeCompressor({.sink = sink}));
}

TEST(plugins_webp_LibWebpCompressor, whenWrittingUnsupportedPixel_thenThrow) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor = makeCompressor({.sink = sink});

  ASSERT_THROW(
      compressor.writeScanline(std::make_unique<image::Scanline>(
          image::pixel::specifications::Gray, 10)),
      SpectrumException);
  ASSERT_THROW(
      compressor.writeScanline(std::make_unique<image::Scanline>(
          image::pixel::specifications::RGB, 10)),
      SpectrumException);
  ASSERT_THROW(
      compressor.writeScanline(std::make_unique<image::Scanline>(
          image::pixel::specifications::ARGB, 10)),
      SpectrumException);
}

TEST(plugins_webp_LibWebpCompressor, whenWrittingData_thenOutputValid) {
  auto sink = io::testutils::FakeImageSink{};
  auto compressor =
      makeCompressor({.sink = sink, .imageSize = image::Size{1, 2}});

  compressor.writeScanline(
      std::make_unique<image::Scanline>(image::pixel::specifications::RGBA, 1));

  ASSERT_TRUE(sink.stringContent().empty());

  compressor.writeScanline(
      std::make_unique<image::Scanline>(image::pixel::specifications::RGBA, 1));

  ASSERT_FALSE(sink.stringContent().empty());
}

} // namespace test
} // namespace webp
} // namespace plugins
} // namespace spectrum
} // namespace facebook
