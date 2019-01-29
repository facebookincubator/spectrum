// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Configuration.h"

#include <spectrum/core/SpectrumEnforce.h>

namespace facebook {
namespace spectrum {

#define SPECTRUM_CONFIGURATION_MERGE_PROPERTY(name, other) \
  if (other._##name.hasValue()) {                          \
    _##name = other._##name.value();                       \
  }

#define SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(name, other) \
  _##name == other._##name

bool configurationCurrentPlatformValue(const bool android, const bool ios) {
#if defined(SPECTRUM_TARGET_IOS)
  return ios;
#elif defined(SPECTRUM_TARGET_ANDROID)
  return android;
#elif defined(SPECTRUM_TARGET_XPLAT)
  // Returning android defaults as android/cxx link against the same libraries.
  return android;
#else
#error Unknown platform
#endif
}

//
// General
//

void Configuration::General::merge(const General& rhs) {
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(defaultBackgroundColor, rhs);
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(interpretMetadata, rhs);
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(
      propagateChromaSamplingModeFromSource, rhs);
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(chromaSamplingModeOverride, rhs);
}

bool Configuration::General::operator==(const General& rhs) const {
  return SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(defaultBackgroundColor, rhs) &&
      SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(interpretMetadata, rhs) &&
      SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(
             propagateChromaSamplingModeFromSource, rhs) &&
      SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(chromaSamplingModeOverride, rhs);
}

std::string Configuration::General::chromaSamplingModeOverrideStringFromValue(
    const ChromaSamplingModeOverride chromaSamplingModeOverride) {
  switch (chromaSamplingModeOverride) {
    case ChromaSamplingModeOverride::None:
      return "none";
    case ChromaSamplingModeOverride::S444:
      return "S444";
    case ChromaSamplingModeOverride::S420:
      return "S420";
    case ChromaSamplingModeOverride::S422:
      return "S422";
    case ChromaSamplingModeOverride::S440:
      return "S440";
    case ChromaSamplingModeOverride::S411:
      return "S411";
    default:
      return core::makeUnknownWithValue<std::uint8_t>(
          chromaSamplingModeOverride);
  }
}

Configuration::General::ChromaSamplingModeOverride
Configuration::General::chromaSamplingModeOverrideFromChromaSamplingMode(
    const image::ChromaSamplingMode chromaSamplingMode) {
  switch (chromaSamplingMode) {
    case image::ChromaSamplingMode::S444:
      return Configuration::General::ChromaSamplingModeOverride::S444;
    case image::ChromaSamplingMode::S420:
      return Configuration::General::ChromaSamplingModeOverride::S420;
    case image::ChromaSamplingMode::S422:
      return Configuration::General::ChromaSamplingModeOverride::S422;
    case image::ChromaSamplingMode::S440:
      return Configuration::General::ChromaSamplingModeOverride::S440;
    case image::ChromaSamplingMode::S411:
      return Configuration::General::ChromaSamplingModeOverride::S411;
  }
}

folly::Optional<image::ChromaSamplingMode>
Configuration::General::chromaSamplingModeOverrideToChromaSamplingMode(
    const Configuration::General::ChromaSamplingModeOverride
        chromaSamplingModeOverride) {
  switch (chromaSamplingModeOverride) {
    case Configuration::General::ChromaSamplingModeOverride::None:
      return folly::none;
    case Configuration::General::ChromaSamplingModeOverride::S444:
      return image::ChromaSamplingMode::S444;
    case Configuration::General::ChromaSamplingModeOverride::S420:
      return image::ChromaSamplingMode::S420;
    case Configuration::General::ChromaSamplingModeOverride::S422:
      return image::ChromaSamplingMode::S422;
    case Configuration::General::ChromaSamplingModeOverride::S440:
      return image::ChromaSamplingMode::S440;
    case Configuration::General::ChromaSamplingModeOverride::S411:
      return image::ChromaSamplingMode::S411;
  }
}

Configuration::General::ChromaSamplingModeOverride
Configuration::General::chromaSamplingModeOverrideFromValue(const int value) {
  const auto chromaSamplingModeOverride =
      static_cast<ChromaSamplingModeOverride>(value);
  switch (chromaSamplingModeOverride) {
    case ChromaSamplingModeOverride::None:
    case ChromaSamplingModeOverride::S444:
    case ChromaSamplingModeOverride::S420:
    case ChromaSamplingModeOverride::S422:
    case ChromaSamplingModeOverride::S440:
    case ChromaSamplingModeOverride::S411:
      return chromaSamplingModeOverride;
    default:
      SPECTRUM_UNREACHABLE_CONFIGURATION_CHROMA_SAMPLING_MODE_OVERRIDE(
          chromaSamplingModeOverride);
  }
}

Configuration::General::SamplingMethod
Configuration::General::makeSamplingMethodFromValue(const int value) {
  SPECTRUM_ENFORCE_IF_NOT(
      value >=
          static_cast<int>(Configuration::General::SamplingMethod::Bicubic) &&
      value <= static_cast<int>(
                   Configuration::General::SamplingMethod::MagicKernel));
  return static_cast<Configuration::General::SamplingMethod>(value);
}

std::string Configuration::General::samplingMethodStringFromValue(
    const Configuration::General::SamplingMethod samplingMethod) {
  switch (samplingMethod) {
    case SamplingMethod::Bicubic:
      return "bicubic";
    case SamplingMethod::MagicKernel:
      return "magic_kernel";
    default:
      return core::makeUnknownWithValue<std::uint8_t>(samplingMethod);
  }
}

//
// Jpeg
//

void Configuration::Jpeg::merge(const Jpeg& rhs) {
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(useTrellis, rhs);
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(useProgressive, rhs);
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(useOptimizeScan, rhs);
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(useCompatibleDcScanOpt, rhs);
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(usePsnrQuantTable, rhs);
}

bool Configuration::Jpeg::operator==(const Jpeg& rhs) const {
  return SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(useTrellis, rhs) &&
      SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(useProgressive, rhs) &&
      SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(useOptimizeScan, rhs) &&
      SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(useCompatibleDcScanOpt, rhs) &&
      SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(usePsnrQuantTable, rhs);
}

//
// Png
//

const Configuration::Png::CompressionLevel
    Configuration::Png::CompressionLevelNone = 0;
const Configuration::Png::CompressionLevel
    Configuration::Png::CompressionLevelBestSpeed = 1;
const Configuration::Png::CompressionLevel
    Configuration::Png::CompressionLevelBestCompression = 9;
const Configuration::Png::CompressionLevel
    Configuration::Png::CompressionLevelDefault = -1;

void Configuration::Png::merge(const Png& rhs) {
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(useInterlacing, rhs);
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(compressionLevel, rhs);
}

bool Configuration::Png::operator==(const Png& rhs) const {
  return SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(useInterlacing, rhs) &&
      SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(compressionLevel, rhs);
}

//
// Webp
//

void Configuration::Webp::merge(const Webp& rhs) {
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(method, rhs);
  SPECTRUM_CONFIGURATION_MERGE_PROPERTY(imageHint, rhs);
}

bool Configuration::Webp::operator==(const Webp& rhs) const {
  return SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(method, rhs) &&
      SPECTRUM_CONFIGURATION_COMPARE_PROPERTY(imageHint, rhs);
}

std::string Configuration::Webp::imageHintStringFromValue(
    const ImageHint imageHint) {
  switch (imageHint) {
    case ImageHint::Default:
      return "default";
    case ImageHint::Picture:
      return "picture";
    case ImageHint::Graph:
      return "graph";
    case ImageHint::Photo:
      return "photo";
    default:
      return core::makeUnknownWithValue<std::uint8_t>(imageHint);
  }
}

Configuration::Webp::ImageHint Configuration::Webp::makeImageHintFromValue(
    const int value) {
  SPECTRUM_ENFORCE_IF_NOT(
      value >= static_cast<int>(Configuration::Webp::ImageHint::Default) &&
      value <= static_cast<int>(Configuration::Webp::ImageHint::Graph));
  return static_cast<Configuration::Webp::ImageHint>(value);
}

//
// Configuration
//

void Configuration::merge(const Configuration& rhs) {
  general.merge(rhs.general);
  jpeg.merge(rhs.jpeg);
  png.merge(rhs.png);
  webp.merge(rhs.webp);
}

bool Configuration::operator==(const Configuration& rhs) const {
  return general == rhs.general && jpeg == rhs.jpeg && png == rhs.png &&
      webp == rhs.webp;
}

bool Configuration::operator!=(const Configuration& rhs) const {
  return !(*this == rhs);
}

void Configuration::mergeInto(const Configuration& rhs) {
  auto result = rhs;
  result.merge(*this);
  *this = result;
}

} // namespace spectrum
} // namespace facebook
