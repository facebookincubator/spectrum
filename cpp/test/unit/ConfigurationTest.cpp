// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/Configuration.h>

#include <folly/Optional.h>
#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace test {

namespace {
template <typename T>
void testEqualityAndComparison(
    const T& nonDefaultValue,
    std::function<T(const Configuration&)> getConfigurationValue,
    std::function<void(Configuration&, const T&)> setOtherConfigurationValue) {
  auto configuration = Configuration{};
  auto configuration2 = Configuration{};

  // Ensure we're not passing the default
  ASSERT_NE(getConfigurationValue(configuration), nonDefaultValue);

  // Ensure property is taken into account in equality
  setOtherConfigurationValue(configuration2, nonDefaultValue);
  ASSERT_EQ(getConfigurationValue(configuration2), nonDefaultValue);
  ASSERT_NE(configuration, configuration2);

  // Ensure property is take into account during merge
  configuration.merge(configuration2);
  ASSERT_EQ(configuration, configuration2);
  ASSERT_EQ(getConfigurationValue(configuration), nonDefaultValue);
}
} // namespace

#define SPECTRUM_CONFIGURATION_TEST_PROPERTY(type, name, nonDefaultValue)      \
  testEqualityAndComparison<type>(                                             \
      nonDefaultValue,                                                         \
      [](const Configuration& configuration) { return configuration.name(); }, \
      [](Configuration& configuration, const type& value) {                    \
        configuration.name(value);                                             \
      });

//
// Parameters
//

TEST(Configuration, whenRequestingDefaultValue_allParametersAreCorrect) {
  const auto configuration = Configuration{};

  // General
  ASSERT_EQ(
      image::Color({255, 255, 255}),
      configuration.general.defaultBackgroundColor());
  ASSERT_TRUE(configuration.general.interpretMetadata());
  ASSERT_TRUE(configuration.general.propagateChromaSamplingModeFromSource());
  ASSERT_EQ(
      Configuration::General::ChromaSamplingModeOverride::None,
      configuration.general.chromaSamplingModeOverride());

  // Jpeg
  ASSERT_TRUE(configuration.jpeg.useTrellis());
  ASSERT_EQ(
      configurationCurrentPlatformValue(true, false),
      configuration.jpeg.useProgressive());
  ASSERT_FALSE(configuration.jpeg.useOptimizeScan());
  ASSERT_EQ(
      configurationCurrentPlatformValue(true, false),
      configuration.jpeg.useCompatibleDcScanOpt());
  ASSERT_EQ(false, configuration.jpeg.usePsnrQuantTable());

  // Png
  ASSERT_EQ(false, configuration.png.useInterlacing());
  ASSERT_EQ(
      Configuration::Png::CompressionLevelDefault,
      configuration.png.compressionLevel());

  // WebP
  ASSERT_EQ(3, configuration.webp.method());
  ASSERT_EQ(
      Configuration::Webp::ImageHint::Default, configuration.webp.imageHint());
}

TEST(
    Configuration_General,
    whenMergingOrComparing_thenDefaultBackgroundColorIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(
      image::Color, general.defaultBackgroundColor, image::Color({0, 0, 0}));
}

TEST(
    Configuration_General,
    whenMergingOrComparing_thenInterpretMetadataIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(bool, general.interpretMetadata, false);
}

TEST(
    Configuration_General,
    whenMergingOrComparing_thenpropagateChromaSamplingModeFromSourceIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(
      bool, general.propagateChromaSamplingModeFromSource, false);
}

TEST(
    Configuration_General,
    whenMergingOrComparing_thenChromaSamplingModeIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(
      Configuration::General::ChromaSamplingModeOverride,
      general.chromaSamplingModeOverride,
      Configuration::General::ChromaSamplingModeOverride::S444);
}

TEST(Configuration_Jpeg, whenMergingOrComparing_thenUseTrellisIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(bool, jpeg.useTrellis, false);
}

TEST(
    Configuration_Jpeg,
    whenMergingOrComparing_thenUseProgressiveIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(
      bool,
      jpeg.useProgressive,
      configurationCurrentPlatformValue(false, true));
}

TEST(
    Configuration_Jpeg,
    whenMergingOrComparing_thenUseOptimizeScanIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(bool, jpeg.useOptimizeScan, true);
}

TEST(
    Configuration_Jpeg,
    whenMergingOrComparing_thenUseCompatibleDcScanOptIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(
      bool,
      jpeg.useCompatibleDcScanOpt,
      configurationCurrentPlatformValue(false, true));
}

TEST(
    Configuration_Jpeg,
    whenMergingOrComparing_thenUsePsnrQuantTableIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(bool, jpeg.usePsnrQuantTable, true);
}

TEST(
    Configuration_Png,
    whenMergingOrComparing_thenUseInterlacingIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(bool, png.useInterlacing, true);
}

TEST(
    Configuration_Png,
    whenMergingOrComparing_thenCompressionLevelIsAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(
      Configuration::Png::CompressionLevel,
      png.compressionLevel,
      Configuration::Png::CompressionLevelBestCompression);
}

TEST(Configuration_WebP, whenMergingOrComparing_thenMethodAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(int, webp.method, 6);
}

TEST(Configuration_WebP, whenMergingOrComparing_thenImageHintAccountedFor) {
  SPECTRUM_CONFIGURATION_TEST_PROPERTY(
      Configuration::Webp::ImageHint,
      webp.imageHint,
      Configuration::Webp::ImageHint::Graph);
}

} // namespace test
} // namespace spectrum
} // namespace facebook
