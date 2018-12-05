// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Specification.h"

#include <sstream>

namespace facebook {
namespace spectrum {
namespace image {

std::string chromaSamplingModeStringValue(
    const folly::Optional<ChromaSamplingMode>& chromaSamplingMode) {
  if (chromaSamplingMode.hasValue()) {
    switch (*chromaSamplingMode) {
      case ChromaSamplingMode::S444:
        return "444";
      case ChromaSamplingMode::S420:
        return "420";
      case ChromaSamplingMode::S422:
        return "422";
      case ChromaSamplingMode::S411:
        return "411";
      case ChromaSamplingMode::S440:
        return "440";
    }
  } else {
    return "none";
  }
}

ChromaSamplingMode chromaSamplingModeFromValue(const int value) {
  const auto chromaSamplingMode = static_cast<ChromaSamplingMode>(value);

  switch (chromaSamplingMode) {
    case image::ChromaSamplingMode::S444:
    case image::ChromaSamplingMode::S420:
    case image::ChromaSamplingMode::S422:
    case image::ChromaSamplingMode::S411:
    case image::ChromaSamplingMode::S440:
      return chromaSamplingMode;
    default:
      SPECTRUM_UNREACHABLE_IMAGE_CHROMA_SAMPLING_MODE(chromaSamplingMode);
  }
}

std::string Specification::string() const {
  std::stringstream ss;
  ss << "{size:" << size.string() << ","
     << "format:" << format.string() << ","
     << "pixelSpecification:" << pixelSpecification.string() << ","
     << "orientation:" << orientationStringFromValue(orientation) << ","
     << "chromaSamplingMode:"
     << chromaSamplingModeStringValue(chromaSamplingMode) << ","
     << "metadata:" << metadata.string() << "}";
  return ss.str();
}

bool Specification::operator==(const Specification& rhs) const {
  return size == rhs.size && format == rhs.format &&
      orientation == rhs.orientation &&
      pixelSpecification == rhs.pixelSpecification &&
      chromaSamplingMode == rhs.chromaSamplingMode && metadata == rhs.metadata;
}

bool Specification::operator!=(const Specification& rhs) const {
  return !(*this == rhs);
}

} // namespace image
} // namespace spectrum
} // namespace facebook
