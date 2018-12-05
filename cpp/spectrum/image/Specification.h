// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Format.h>
#include <spectrum/image/Geometry.h>
#include <spectrum/image/Metadata.h>
#include <spectrum/image/Orientation.h>
#include <spectrum/image/Pixel.h>

#include <folly/Optional.h>

#include <cstdint>
#include <string>

namespace facebook {
namespace spectrum {
namespace image {

/**
 * Chroma sampling describe the number of luma information per chroma
 * information.
 */
enum class ChromaSamplingMode {
  S444 = 0,
  S420 = 1,
  S422 = 2,
  S411 = 3,
  S440 = 4,
};

#define SPECTRUM_UNREACHABLE_IMAGE_CHROMA_SAMPLING_MODE(chromaSamplingMode) \
  SPECTRUM_UNREACHABLE_STRING(                                              \
      folly::StringPiece{"unreachable_chroma_sampling_mode"},               \
      chromaSamplingModeStringValue(chromaSamplingMode))

std::string chromaSamplingModeStringValue(
    const folly::Optional<ChromaSamplingMode>& chromaSamplingMode);

ChromaSamplingMode chromaSamplingModeFromValue(const int value);

/** An image's specification contains all derived and extracted information
 * besides the pixel content. */
struct Specification {
 public:
  Size size;
  Format format;
  pixel::Specification pixelSpecification;
  Orientation orientation{Orientation::Up};
  folly::Optional<ChromaSamplingMode> chromaSamplingMode;
  Metadata metadata{};

  std::string string() const;
  bool operator==(const Specification& rhs) const;
  bool operator!=(const Specification& rhs) const;
};

} // namespace image
} // namespace spectrum
} // namespace facebook
