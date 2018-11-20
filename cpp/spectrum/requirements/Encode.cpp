// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Encode.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <sstream>

namespace facebook {
namespace spectrum {
namespace requirements {

constexpr Encode::Quality Encode::QualityUnset;
constexpr Encode::Quality Encode::QualityMin;
constexpr Encode::Quality Encode::QualityDefault;
constexpr Encode::Quality Encode::QualityMax;

std::string Encode::modeStringFromValue(const Mode mode) {
  switch (mode) {
    case Mode::Lossless:
      return "lossless";
    case Mode::Lossy:
      return "lossy";
    case Mode::Any:
      return "any";
    default:
      return core::makeUnknownWithValue<std::uint8_t>(mode);
  }
}

Encode::Mode Encode::modeFromValue(const int value) {
  SPECTRUM_ENFORCE_IF_NOT(
      value >= static_cast<int>(Mode::Lossless) &&
      value <= static_cast<int>(Mode::Any));
  return static_cast<Mode>(value);
}

void Encode::validate() const {
  SPECTRUM_ENFORCE_IF(quality > QualityMax);
}

bool Encode::operator==(const Encode& rhs) const {
  return format == rhs.format && quality == rhs.quality && mode == rhs.mode;
}

bool Encode::operator!=(const Encode& rhs) const {
  return !(*this == rhs);
}

bool Encode::hasQuality() const {
  return quality != QualityUnset;
}

Encode::Quality Encode::sanitizedQuality(
    const Quality defaultValue,
    const Quality min,
    const Quality max) const {
  if (quality == QualityUnset) {
    return defaultValue;
  } else {
    return std::max(std::min(quality, max), min);
  }
}

std::string Encode::string() const {
  std::stringstream ss;
  ss << "{format:\"" << format.identifier() << "\",";
  ss << "quality:\"" << quality << "\",";
  ss << "mode:\"" << modeStringFromValue(mode) << "\"";
  ss << "}";
  return ss.str();
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
