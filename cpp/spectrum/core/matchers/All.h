// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Rule.h>
#include <spectrum/core/Operation.h>
#include <spectrum/core/matchers/ResultMatcher.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace matchers {
namespace reasons {
extern const folly::StringPiece InputImageFormatMissing;
extern const folly::StringPiece InputImageFormatUnsupported;
extern const folly::StringPiece OutputImageFormatMissing;
extern const folly::StringPiece OutputImageFormatUnsupported;
extern const folly::StringPiece ResizeUnsupported;
extern const folly::StringPiece CropUnsupported;
extern const folly::StringPiece EqualInputOutputImageFormatFalse;
extern const folly::StringPiece PassthroughDenied;
extern const folly::StringPiece RotateUnsupported;
} // namespace reasons

/**
 * Characteristic matcher of a rule's allowedInputFormats.
 *
 * @param rule The rule to test.
 * @param parameters The operation's parameters to match.
 * @return folly::none if it matches - otherwise the failure reason.
 */
Result matchesAllowedInputImageFormat(
    const Rule& rule,
    const Operation::Parameters& parameters);

/**
 * Characteristic matcher of a rule's allowedOutputFormats.
 *
 * @param rule The rule to test.
 * @param parameters The operation's parameters to match.
 * @return folly::none if it matches - otherwise the failure reason.
 */
Result matchesAllowedOutputImageFormats(
    const Rule& rule,
    const Operation::Parameters& parameters);

/**
 * Characteristic matcher of a rule's requiresEqualInputOutputFormat.
 *
 * @param rule The rule to test.
 * @param parameters The operation's parameters to match.
 * @return folly::none if it matches - otherwise the failure reason.
 */
Result matchesEqualInputOutputImageFormatRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters);

/**
 * Characteristic matcher of a rule's passthrough-ness.
 *
 * @param rule The rule to test.
 * @param parameters The operation's parameters to match.
 * @return folly::none if it matches - otherwise the failure reason.
 */
Result matchesPassthroughRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters);

/**
 * Characteristic matcher of a rule's resizing support.
 *
 * @param rule The rule to test.
 * @param parameters The operation's parameters to match.
 * @return folly::none if it matches - otherwise the failure reason.
 */
Result matchesResizeRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters);

/**
 * Characteristic matcher of a rule's cropping support.
 *
 * @param rule The rule to test.
 * @param parameters The operation's parameters to match.
 * @return folly::none if it matches - otherwise the failure reason.
 */
Result matchesCropRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters);

/**
 * Characteristic matcher of a rule's rotation support.
 *
 * @param rule The rule to test.
 * @param parameters The operation's parameters to match.
 * @return folly::none if it matches - otherwise the failure reason.
 */
Result matchesRotateRequirement(
    const Rule& rule,
    const Operation::Parameters& parameters);

} // namespace matchers
} // namespace core
} // namespace spectrum
} // namespace facebook
