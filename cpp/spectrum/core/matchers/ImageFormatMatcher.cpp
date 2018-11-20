// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "All.h"

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {
namespace reasons {

const folly::StringPiece InputImageFormatUnsupported{
    "characteristic_matcher_input_format_unsupported"};
const folly::StringPiece EqualInputOutputImageFormatFalse{
    "characteristic_matcher_equal_input_output_format_false"};
const folly::StringPiece OutputImageFormatUnsupported{
    "characteristic_matcher_output_format_unsupported"};

} // namespace reasons

namespace {

Result matchesAllowedImageFormat(
    const std::vector<image::Format>& allowedImageFormat,
    const image::Format& imageFormat,
    const Result& unsupportedFailureResult) {
  const auto allowedImageFormatIterator = std::find(
      allowedImageFormat.cbegin(), allowedImageFormat.cend(), imageFormat);
  if (allowedImageFormat.empty() ||
      allowedImageFormatIterator != allowedImageFormat.cend()) {
    return Result::ok();
  } else {
    return unsupportedFailureResult;
  }
}
} // namespace

Result matchesEqualInputOutputImageFormatRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters) {
  const auto inputFormat = parameters.inputImageSpecification.format;
  const auto outputFormat = parameters.outputImageFormat;

  if (rule.requiresEqualInputOutputFormat && inputFormat != outputFormat) {
    return reasons::EqualInputOutputImageFormatFalse;
  }

  return Result::ok();
}

Result matchesAllowedInputImageFormat(
    const Rule& rule,
    const Operation::Parameters& parameters) {
  const auto inputFormat = parameters.inputImageSpecification.format;

  return matchesAllowedImageFormat(
      rule.allowedInputFormats,
      inputFormat,
      reasons::InputImageFormatUnsupported);
}

Result matchesAllowedOutputImageFormats(
    const Rule& rule,
    const Operation::Parameters& parameters) {
  return matchesAllowedImageFormat(
      rule.allowedOutputFormats,
      parameters.outputImageFormat,
      reasons::OutputImageFormatUnsupported);
}

} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
