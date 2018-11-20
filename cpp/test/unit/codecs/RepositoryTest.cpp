// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/codecs/Repository.h>
#include <spectrum/image/metadata/Entries.h>
#include <spectrum/testutils/TestUtils.h>

#include <exception>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace test {

namespace {
class FakeCompressor final : public ICompressor {
 public:
  std::string identifier;
  FakeCompressor(const std::string identifier) : identifier(identifier) {}

  void writeScanline(std::unique_ptr<image::Scanline> scanline) override{};
};

class FakeDecompressor final : public IDecompressor {
 public:
  std::string identifier;
  FakeDecompressor(const std::string identifier) : identifier(identifier) {}

  // no-op implementations
  image::Specification sourceImageSpecification() override {
    throw std::runtime_error("");
  };

  image::Specification outputImageSpecification() override {
    throw std::runtime_error("");
  };

  std::unique_ptr<image::Scanline> readScanline() override {
    throw std::runtime_error("");
  };
};

CompressorProvider makeFakeCompressorProvider(
    const std::string& identifier,
    const image::Format& imageFormat) {
  CompressorProvider::Factory fakeCompressorFactory =
      [identifier](const codecs::CompressorOptions& /* unused */) {
        return std::make_unique<FakeCompressor>(identifier);
      };
  return {
      .format = imageFormat,
      .supportsSettingMetadata = true,
      .compressorFactory = fakeCompressorFactory,
  };
}

DecompressorProvider makeFakeDecompressorProvider(
    const std::string& identifier,
    const image::Format& imageFormat) {
  DecompressorProvider::Factory fakeDecompressorFactory =
      [identifier](
          io::IImageSource& source,
          const folly::Optional<image::Ratio>& samplingRatio,
          const Configuration configuration) {
        return std::make_unique<FakeDecompressor>(identifier);
      };
  return {
      .format = imageFormat,
      .decompressorFactory = fakeDecompressorFactory,
  };
}
} // namespace

TEST(
    codecs_Repository,
    whenAddedMultipleCompressorFactories_thenMatchingCompressorIsReturened) {
  Repository codecRepository;

  codecRepository.addCompressorProvider(
      makeFakeCompressorProvider("pngCompressor", image::formats::Png));
  codecRepository.addCompressorProvider(
      makeFakeCompressorProvider("jpegCompressor", image::formats::Jpeg));
  codecRepository.addCompressorProvider(
      makeFakeCompressorProvider("webpCompressor", image::formats::Webp));

  auto sink = io::testutils::FakeImageSink{};
  const auto compressor =
      codecRepository.compressorProvider(image::formats::Jpeg)
          .compressorFactory(codecs::CompressorOptions{
              .sink = sink,
              .imageSpecification =
                  image::testutils::makeDummyImageSpecification(
                      image::formats::Png),
              .encodeRequirement = folly::none,
              .configuration = Configuration{},
          });

  ASSERT_EQ(
      "jpegCompressor",
      static_cast<FakeCompressor*>(compressor.get())->identifier);
}

TEST(
    codecs_Repository,
    whenAddedMultipleDecompressorFactories_thenMatchingDecompressorIsReturened) {
  Repository codecRepository;

  codecRepository.addDecompressorProvider(
      makeFakeDecompressorProvider("pngDecompressor", image::formats::Png));
  codecRepository.addDecompressorProvider(
      makeFakeDecompressorProvider("webpDecompressor", image::formats::Webp));
  codecRepository.addDecompressorProvider(
      makeFakeDecompressorProvider("jpegDecompressor", image::formats::Jpeg));

  auto source = io::testutils::makeVectorImageSource("");
  const auto decompressor =
      codecRepository.decompressorProvider(image::formats::Webp)
          .decompressorFactory(
              source,
              folly::none,
              Configuration());

  ASSERT_EQ(
      "webpDecompressor",
      static_cast<FakeDecompressor*>(decompressor.get())->identifier);
}

} // namespace test
} // namespace codecs
} // namespace spectrum
} // namespace facebook
