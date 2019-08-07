// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/codecs/EncodedImageSpecificationDetector.h>
#include <spectrum/testutils/TestUtils.h>

#include <folly/Optional.h>
#include <gtest/gtest.h>

using namespace facebook::spectrum::core;

namespace facebook {
namespace spectrum {
namespace codecs {
namespace test {
namespace {

image::Specification detectImageSpecification(
    io::IImageSource& source,
    const Options& options,
    const image::Size& decompressorImageSize = {2, 2},
    const image::pixel::Specification& decompressorPixelSpecification =
        image::pixel::specifications::Gray,
    std::vector<EncodedImageFormatDetectorHandler>&&
        encodedImageFormatDetectorHandlers = {}) {
  const auto codecRepository =
      codecs::testutils::makeCodecRepositoryWithFakeDecompressor(
          decompressorImageSize, decompressorPixelSpecification);
  Configuration c;
  const auto encodedImageInformationDetector =
      EncodedImageSpecificationDetector{
          codecRepository,
          c,
          std::move(encodedImageFormatDetectorHandlers),
      };

  auto rewindableImageSource = io::RewindableImageSource{source};
  return encodedImageInformationDetector.detectImageSpecification(
      rewindableImageSource, options);
}

} // namespace

TEST(
    codecs_EncodedImageSpecificationDetector,
    whenInputHasNoSpecificationAndCanDetect_thenSpecificationReturnedAndUpdated) {
  const auto size = image::Size{320, 240};
  const auto format = image::formats::Jpeg;
  const auto pixelSpecification = image::pixel::specifications::RGB;

  auto options = spectrum::testutils::makeDummyTranscodeOptions();
  auto source = io::testutils::makeVectorImageSource("abc");
  auto didRunHandlerOne = false;
  const auto handlerOne =
      testutils::makeEncodedImageFormatDetectorHandlerReturning(
          format, didRunHandlerOne);

  const auto specification = detectImageSpecification(
      source, options, size, pixelSpecification, {handlerOne});

  // Fake EncodeImageFormatDetector::Handler doesn't read source purposely.
  ASSERT_EQ(0, source.getTotalBytesRead());

  // Assert on return value
  ASSERT_EQ(size, specification.size);
  ASSERT_EQ(format, specification.format);
  ASSERT_EQ(pixelSpecification, specification.pixelSpecification);
}

} // namespace test
} // namespace codecs
} // namespace spectrum
} // namespace facebook
