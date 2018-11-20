// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/codecs/EncodedImageFormatDetector.h>
#include <spectrum/io/RewindableImageSource.h>
#include <spectrum/testutils/TestUtils.h>

#include <folly/Optional.h>
#include <folly/Range.h>
#include <gtest/gtest.h>

#include <array>
#include <cstring>
#include <functional>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace test {
namespace {

void testAndAssertHandlerWithContentSizeDelta(const int contentSizeDelta) {
  const auto header = std::string("sample_header");
  const auto handler =
      makeSimpleImageFormatDetectorHandler(header, image::formats::Jpeg);
  const auto sourceContent = contentSizeDelta < 0
      ? header.substr(0, header.size() + contentSizeDelta)
      : header + std::string(contentSizeDelta, '_');
  auto source = io::testutils::makeVectorImageSource(sourceContent);
  const auto detectedFormat = handler(source);

  if (contentSizeDelta >= 0) {
    ASSERT_EQ(image::formats::Jpeg, detectedFormat);
    ASSERT_EQ(header.size(), source.getTotalBytesRead());
  } else {
    ASSERT_EQ(sourceContent.size(), source.getTotalBytesRead());
    ASSERT_EQ(folly::none, handler(source));
  }
}

void testAndAssertImageFormatDetectionWithHandlers(
    std::vector<EncodedImageFormatDetectorHandler>&& handlers,
    const folly::Optional<image::Format>& expectedFormat) {
  const auto imageFormatDetector =
      EncodedImageFormatDetector(std::move(handlers));
  const auto sourceContent = "abcdefghijklmn";
  constexpr auto sourceContentLength = 14;
  auto source = io::testutils::makeVectorImageSource(sourceContent);
  auto rewindableImageSource = io::RewindableImageSource{source};
  const auto result = imageFormatDetector.detectFormat(rewindableImageSource);
  const auto sourceBytesRead = source.getTotalBytesRead();

  // Ensure source has been properly reset to the beginning.
  auto buffer = std::array<char, sourceContentLength>{};
  const auto bytesRead = source.read(buffer.data(), sourceContentLength);
  const auto didResetToBegining = sourceContentLength == bytesRead &&
      std::strncmp(buffer.data(), sourceContent, sourceContentLength) == 0;

  ASSERT_EQ(expectedFormat, result);
  ASSERT_EQ(0, sourceBytesRead);
  ASSERT_TRUE(didResetToBegining);
}

} // namespace

TEST(
    codecs_EncodedImageFormatDetector,
    whenNoHandlersPassed_thenErrorIsThrown) {
  ASSERT_SPECTRUM_THROW(
      testAndAssertImageFormatDetectionWithHandlers({}, folly::none),
      error::EncodedImageFormatNotDetected);
}

TEST(
    codecs_EncodedImageFormatDetector,
    whenHandlerNotSuccessful_thenErrorIsThrown) {
  auto didRunHandlerOne = false;
  const auto handlerOne =
      testutils::makeEncodedImageFormatDetectorHandlerReturning(
          folly::none, didRunHandlerOne);

  ASSERT_SPECTRUM_THROW(
      testAndAssertImageFormatDetectionWithHandlers({handlerOne}, folly::none),
      error::EncodedImageFormatNotDetected);
  ASSERT_TRUE(didRunHandlerOne);
}

TEST(
    codecs_EncodedImageFormatDetector,
    whenHandlerSuccessful_thenReturnsFormat) {
  auto didRunHandlerOne = false;
  const auto handlerOne =
      testutils::makeEncodedImageFormatDetectorHandlerReturning(
          image::formats::Jpeg, didRunHandlerOne);

  testAndAssertImageFormatDetectionWithHandlers(
      {handlerOne}, image::formats::Jpeg);

  ASSERT_TRUE(didRunHandlerOne);
}

TEST(
    codecs_EncodedImageFormatDetector,
    whenTwoHandlersPassedFirstSuccessful_thenSecondUntouched) {
  auto didRunHandlerOne = false;
  const auto handlerOne =
      testutils::makeEncodedImageFormatDetectorHandlerReturning(
          image::formats::Jpeg, didRunHandlerOne);
  auto didRunHandlerTwo = false;
  const auto handlerTwo =
      testutils::makeEncodedImageFormatDetectorHandlerReturning(
          folly::none, didRunHandlerTwo);

  testAndAssertImageFormatDetectionWithHandlers(
      {handlerOne, handlerTwo}, image::formats::Jpeg);

  ASSERT_TRUE(didRunHandlerOne);
  ASSERT_FALSE(didRunHandlerTwo);
}

TEST(
    codecs_EncodedImageFormatDetector,
    whenTwoHandlersPassedSecondSuccessful_thenBothExecuted) {
  auto didRunHandlerOne = false;
  const auto handlerOne =
      testutils::makeEncodedImageFormatDetectorHandlerReturning(
          folly::none, didRunHandlerOne);
  auto didRunHandlerTwo = false;
  const auto handlerTwo =
      testutils::makeEncodedImageFormatDetectorHandlerReturning(
          image::formats::Jpeg, didRunHandlerTwo);

  testAndAssertImageFormatDetectionWithHandlers(
      {handlerOne, handlerTwo}, image::formats::Jpeg);

  ASSERT_TRUE(didRunHandlerOne);
  ASSERT_TRUE(didRunHandlerTwo);
}

TEST(
    codecs_EncodedImageFormatDetector_Handler,
    whenShorterContentPassedToHandler_thenReturnNoneAndSourceAllRead) {
  testAndAssertHandlerWithContentSizeDelta(-1);
}

TEST(
    codecs_EncodedImageFormatDetector_Handler,
    whenLongerContentPassedToHandler_thenReturnFormatAndSourcePrefixRead) {
  testAndAssertHandlerWithContentSizeDelta(4);
}

TEST(
    codecs_EncodedImageFormatDetector_Handler,
    whenExactContentPassedToHandler_thenReturnFormatAndSourceEntirelyRead) {
  testAndAssertHandlerWithContentSizeDelta(0);
}

TEST(
    codecs_EncodedImageFormatDetector_Handler,
    whenEmptyHeaderPassedToHandlerBuilder_thenThrows) {
  ASSERT_THROW(
      makeSimpleImageFormatDetectorHandler("", image::formats::Jpeg),
      SpectrumException);
}

} // namespace test
} // namespace codecs
} // namespace spectrum
} // namespace facebook
