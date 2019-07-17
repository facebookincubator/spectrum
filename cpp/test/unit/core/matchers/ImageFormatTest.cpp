// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/matchers/All.h>

#include <spectrum/testutils/TestUtils.h>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {
namespace test {
namespace {
auto inputImageFormatFailureReason(
    const image::Format& imageFormat,
    const folly::Optional<image::Format>& allowedFormat) {
  auto parameters = testutils::makeDummyOperationParameters();
  parameters.inputImageSpecification =
      image::testutils::makeDummyImageSpecification(imageFormat);

  auto rule = Rule{.name = "rule"};
  if (allowedFormat.hasValue()) {
    rule.allowedInputFormats = {*allowedFormat};
  }

  return matchesAllowedInputImageFormat(rule, parameters);
}

auto outputImageFormatFailureReason(
    const image::EncodedFormat& imageFormat,
    const folly::Optional<image::Format>& allowedFormat) {
  auto parameters = testutils::makeDummyOperationParameters();
  parameters.outputImageFormat = imageFormat;

  auto rule = Rule{.name = "rule"};
  if (allowedFormat.hasValue()) {
    rule.allowedOutputFormats = {*allowedFormat};
  }

  return matchesAllowedOutputImageFormats(rule, parameters);
}

auto equalInputOutputImageFormatFailureReason(
    const image::Format& inputFormat,
    const image::EncodedFormat& outputFormat,
    const bool requiresEqualInputOutputFormat,
    const image::pixel::Specification& inputPixelSpecification =
        image::pixel::specifications::RGBA,
    const folly::Optional<image::pixel::Specification>&
        outputPixelSpecificationRequirement = folly::none) {
  auto parameters = testutils::makeDummyOperationParameters(
      image::formats::Bitmap, outputPixelSpecificationRequirement);
  parameters.inputImageSpecification =
      image::testutils::makeDummyImageSpecification(
          inputFormat, inputPixelSpecification);
  parameters.outputImageFormat = outputFormat;

  const auto rule = Rule{
      .name = "rule",
      .requiresEqualInputOutputFormat = requiresEqualInputOutputFormat,
  };

  return matchesEqualInputOutputImageFormatRequirement(rule, parameters);
}
} // namespace

TEST(core_matchers_ImageFormat, whenInputFormatUnsupported_thenFail) {
  ASSERT_EQ(
      reasons::InputImageFormatUnsupported,
      inputImageFormatFailureReason(image::formats::Png, image::formats::Jpeg)
          .failureReason);
}

TEST(core_matchers_ImageFormat, whenInputFormatSupported_thenSucceed) {
  ASSERT_TRUE(
      inputImageFormatFailureReason(image::formats::Jpeg, image::formats::Jpeg)
          .success());
  ASSERT_TRUE(inputImageFormatFailureReason(image::formats::Jpeg, folly::none)
                  .success());
}

TEST(core_matchers_ImageFormat, whenOutputFormatUnsupported_thenFail) {
  ASSERT_EQ(
      reasons::OutputImageFormatUnsupported,
      outputImageFormatFailureReason(image::formats::Png, image::formats::Jpeg)
          .failureReason);
}

TEST(core_matchers_ImageFormat, whenOutputFormatSupported_thenSucceed) {
  ASSERT_TRUE(
      outputImageFormatFailureReason(image::formats::Jpeg, image::formats::Jpeg)
          .success());
  ASSERT_TRUE(outputImageFormatFailureReason(image::formats::Jpeg, folly::none)
                  .success());
}

TEST(
    core_matchers_ImageFormat,
    whenRequiresEqualInputOutput_equal_thenSucceeds) {
  ASSERT_TRUE(equalInputOutputImageFormatFailureReason(
                  image::formats::Jpeg, image::formats::Jpeg, true)
                  .success());
  ASSERT_TRUE(equalInputOutputImageFormatFailureReason(
                  image::formats::Jpeg, image::formats::Jpeg, false)
                  .success());
}

TEST(
    core_matchers_ImageFormat,
    whenRequiresEqualInputOutput_equalFormatsDifferentPixelSpec_thenFails) {
  ASSERT_EQ(
      reasons::EqualInputOutputImageFormatFalse,
      equalInputOutputImageFormatFailureReason(
          image::formats::Png,
          image::formats::Png,
          true,
          image::pixel::specifications::RGBA,
          image::pixel::specifications::Gray)
          .failureReason);
}

TEST(
    core_matchers_ImageFormat,
    whenRequiresEqualInputOutput_butNotEqual_thenFails) {
  ASSERT_EQ(
      reasons::EqualInputOutputImageFormatFalse,
      equalInputOutputImageFormatFailureReason(
          image::formats::Png, image::formats::Jpeg, true)
          .failureReason);
  ASSERT_EQ(
      reasons::EqualInputOutputImageFormatFalse,
      equalInputOutputImageFormatFailureReason(
          image::formats::Jpeg, image::formats::Png, true)
          .failureReason);
}

} // namespace matchers
} // namespace core
} // namespace test
} // namespace spectrum
} // namespace facebook
