// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Format.h>

#include <string>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace requirements {

/**
 * Defines how an image should be encoded.
 */
struct Encode {
  using Quality = int;

  static constexpr Quality QualityUnset = 0;
  static constexpr Quality QualityMin = 1;
  static constexpr Quality QualityDefault = 75;
  static constexpr Quality QualityMax = 100;

  enum class Mode : std::uint8_t {
    Lossless = 0,
    Lossy = 1,
    Any = 2,
  };

  /**
   * The format of the target encoded image.
   */
  image::EncodedFormat format;

  /**
   * The quality used to encode the image. 0 is unset.
   */
  Quality quality{QualityUnset};

  /**
   * The compression mode.
   */
  Mode mode{Mode::Any};

  /**
   * Throws an exception if the instance isn't valid (such as lossy without
   * a quality set).
   */
  void validate() const;

  /**
   * Returns true if a quality is set.
   */
  bool hasQuality() const;

  /**
   * The quality sanitized within min / max values.
   *
   * @param defaultValue The value to use when quality is unset.
   * @param min The minimum allowed value.
   * @param max The maximum allowed value.
   */
  Quality sanitizedQuality(
      const Quality defaultValue = QualityDefault,
      const Quality min = QualityMin,
      const Quality max = QualityMax) const;

  /**
   * String representation of an encode requirement.
   */
  std::string string() const;

  static std::string modeStringFromValue(const Mode mode);

  static Mode modeFromValue(const int value);

  bool operator==(const Encode& rhs) const;
  bool operator!=(const Encode& rhs) const;
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
