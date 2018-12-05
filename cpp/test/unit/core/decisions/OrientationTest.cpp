// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/decisions/OrientationDecision.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::core::decisions;

namespace facebook {
namespace spectrum {
namespace core {
namespace decisions {
namespace test {
namespace {

Configuration makeConfiguration(const bool interpretMetadata) {
  auto configuration = Configuration{};
  configuration.general.interpretMetadata(interpretMetadata);
  return configuration;
}

} // namespace

static const auto defaultImageSize = image::Size{1, 2};

TEST(decisions_Orientation, whenCreatingWithDefaults_thenValuesCorrect) {
  const auto decision = OrientationDecision::calculate(
      folly::none,
      image::orientationDefault,
      defaultImageSize,
      makeConfiguration(true),
      false);

  ASSERT_EQ(image::orientationDefault, decision.orientation);
  ASSERT_EQ(image::orientationDefault, decision.metadataOrientation);
  ASSERT_EQ(defaultImageSize, decision.size);
}

TEST(
    decisions_Orientation,
    whenPassingBothImageAndMetadataOrientation_thenImageIsPrioritised) {
  const auto decision = OrientationDecision::calculate(
      folly::none,
      image::Orientation::Left,
      defaultImageSize,
      makeConfiguration(true),
      false);

  ASSERT_EQ(image::Orientation::Left, decision.orientation);
  ASSERT_EQ(image::orientationDefault, decision.metadataOrientation);
}

TEST(decisions_Orientation, whenPassingOnlyImageOrientation_thenReturned) {
  const auto decision = OrientationDecision::calculate(
      folly::none,
      image::Orientation::Left,
      defaultImageSize,
      makeConfiguration(true),
      false);

  ASSERT_EQ(image::Orientation::Left, decision.orientation);
  ASSERT_EQ(image::orientationDefault, decision.metadataOrientation);
}

TEST(
    decisions_Orientation,
    whenCompressorAllowsSettingMetadata_thenOrientationEncodedInMetadata) {
  const auto decision = OrientationDecision::calculate(
      folly::none,
      image::Orientation::Left,
      defaultImageSize,
      makeConfiguration(true),
      true);

  ASSERT_EQ(image::orientationDefault, decision.orientation);
  ASSERT_EQ(image::Orientation::Left, decision.metadataOrientation);
  ASSERT_EQ(defaultImageSize, decision.size);
}

TEST(
    decisions_Orientation,
    whenCompressorAllowsSettingMetadataButConfigurationPrevents_thenMetadataNotLeveraged) {
  const auto decision = OrientationDecision::calculate(
      folly::none,
      image::Orientation::Left,
      defaultImageSize,
      makeConfiguration(false),
      true);

  ASSERT_EQ(image::Orientation::Left, decision.orientation);
  ASSERT_EQ(image::orientationDefault, decision.metadataOrientation);
}

TEST(
    decisions_Orientation,
    whenCompressorAllowsSettingMetadataButResizeRequirementPrevents_thenMetadataNotLeveraged) {
  const auto rotateRequirement = requirements::Rotate{
      .forceUpOrientation = true,
  };

  const auto decision = OrientationDecision::calculate(
      rotateRequirement,
      image::Orientation::Left,
      defaultImageSize,
      makeConfiguration(true),
      true);

  ASSERT_EQ(image::Orientation::Left, decision.orientation);
  ASSERT_EQ(image::orientationDefault, decision.metadataOrientation);
}

TEST(decisions_Orientation, whenRotateRequirement_thenTakenIntoAccount) {
  const auto rotateRequirement = requirements::Rotate{.degrees = 90};

  const auto decision = OrientationDecision::calculate(
      rotateRequirement,
      image::Orientation::Right,
      defaultImageSize,
      makeConfiguration(true),
      true);

  ASSERT_EQ(image::orientationDefault, decision.orientation);
  ASSERT_EQ(image::Orientation::Bottom, decision.metadataOrientation);
}

} // namespace test
} // namespace decisions
} // namespace core
} // namespace spectrum
} // namespace facebook
