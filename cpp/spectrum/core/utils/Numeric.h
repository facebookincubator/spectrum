// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/SpectrumEnforce.h>

#include <limits>
#include <type_traits>

namespace facebook {
namespace spectrum {
namespace core {
namespace numeric {

/**
 * Rounding rule when converting a number.
 */
enum class RoundingMode {
  Up,
  Nearest,
  Down,
};

/**
 * Rounds a double to a double.
 *
 * @param value The value to round.
 * @param roundingMode The rounding mode to use.
 */
double rounded(const double value, const RoundingMode roundingMode);

/**
 * Converts a value from In type to Out type. This method will throw
 * SpectrumException if a conversion is out of bounds.
 *
 * @param value The value to convert.
 * @param location The location from which this function is called.
 * @param line The line from which this function is called.
 */
template <typename In, typename Out>
Out convertOrThrow(const In& value, const char* location, const int line) {
  if ((std::is_unsigned<Out>::value && value < 0) ||
      value < std::numeric_limits<Out>::lowest()) {
    SPECTRUM_ERROR_UNDERFLOW(In, Out, location, line);
  } else if (value > std::numeric_limits<Out>::max()) {
    SPECTRUM_ERROR_OVERFLOW(In, Out, location, line);
  } else {
    return static_cast<Out>(value);
  }
}

/**
 * Helper macro to call convertOrThrow without passing the location/line
 * which also infers Out type.
 */
#define SPECTRUM_CONVERT_OR_THROW(from, toType)                              \
  facebook::spectrum::core::numeric::convertOrThrow<decltype(from), toType>( \
      (from), __PRETTY_FUNCTION__, __LINE__)

/**
 * Returns an angle between 0..<360.
 *
 * @param degrees The angle to sanitize.
 */
inline int angleDegreesSanitized(const int degrees) {
  return ((degrees % 360) + 360) % 360;
}

/**
 * Returns true if the value is a multiple of 90.
 *
 * @param value The value to test.
 */
inline int isMultipleOf90(const int value) {
  return (value % 90) == 0;
}

/**
 * Returns true if the value is a multiple of 180.
 *
 * @param value The value to test.
 */
inline int isMultipleOf180(const int value) {
  return (value % 180) == 0;
}

} // namespace numeric
} // namespace core
} // namespace spectrum
} // namespace facebook
