// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/SpectrumException.h>

#include <cstddef>
#include <sstream>
#include <string>

#include <folly/Optional.h>
#include <folly/Range.h>

namespace facebook {
namespace spectrum {
namespace core {

[[noreturn]] void internalThrowUnderflowError(
    const char* location,
    const int line);
[[noreturn]] void internalThrowOverflowError(
    const char* location,
    const int line);

[[noreturn]] void internalThrowError(
    const char* const function,
    const std::uint32_t line,
    const folly::StringPiece& name,
    const char* const extraFormat,
    ...);

template <typename T, typename U>
std::string makeUnknownWithValue(const U& value) {
  std::stringstream s;
  s << "unknown (" << static_cast<T>(value) << ")";
  return s.str();
}
}
} // namespace spectrum
} // namespace facebook

/**
 * SPECTRUM_ENFORCE
 */

#define SPECTRUM_ENFORCE_IF_NOT(condition) \
  SPECTRUM_ERROR_IF_NOT(condition, folly::StringPiece{#condition})

#define SPECTRUM_ENFORCE_IF(condition) \
  SPECTRUM_ERROR_IF(condition, folly::StringPiece{#condition})

/**
 * SPECTRUM_ERROR
 */

#define SPECTRUM_ERROR(name) SPECTRUM_ERROR_IF(true, name)

#define SPECTRUM_ERROR_IF(condition, name) \
  SPECTRUM_ERROR_FORMAT_IF(condition, name, "")

#define SPECTRUM_ERROR_IF_NOT(condition, name) \
  SPECTRUM_ERROR_FORMAT_IF(!(condition), name, "")

#define SPECTRUM_ERROR_STRING_IF(condition, name, string) \
  SPECTRUM_ERROR_FORMAT_IF(condition, name, "%s", string.c_str())

#define SPECTRUM_ERROR_STRING_IF_NOT(condition, name, string) \
  SPECTRUM_ERROR_FORMAT_IF(!(condition), name, "%s", string.c_str())

#define SPECTRUM_ERROR_CSTR_IF(condition, name, string) \
  SPECTRUM_ERROR_FORMAT_IF(condition, name, "%s", string)

#define SPECTRUM_ERROR_CSTR_IF_NOT(condition, name, string) \
  SPECTRUM_ERROR_CSTR_IF(!(condition), name, string)

#define SPECTRUM_ERROR_CSTR(name, string) \
  SPECTRUM_ERROR_CSTR_IF(true, name, string)

#define SPECTRUM_ERROR_STRING(name, string) \
  SPECTRUM_ERROR_STRING_IF(true, name, string)

#define SPECTRUM_ERROR_FORMAT_IF(condition, name, format, ...)         \
  do {                                                                 \
    if (condition) {                                                   \
      facebook::spectrum::core::internalThrowError(                    \
          __PRETTY_FUNCTION__, __LINE__, name, format, ##__VA_ARGS__); \
    }                                                                  \
  } while (false)

#define SPECTRUM_ERROR_FORMAT_IF_NOT(condition, name, format, ...) \
  SPECTRUM_ERROR_FORMAT_IF(!(condition), name, format, ##__VA_ARGS__)

#define SPECTRUM_ERROR_FORMAT(name, format, ...) \
  SPECTRUM_ERROR_FORMAT_IF(true, name, format, ##__VA_ARGS__)

/**
 * SPECTRUM_ERROR_[UNDER/OVER]_FLOW
 */

#define SPECTRUM_ERROR_FLOW(name, from, to, location, line)   \
  do {                                                        \
    facebook::spectrum::core::internalThrowError(             \
        location, line, name, "{from:%s,to:%s}", #from, #to); \
  } while (false)

#define SPECTRUM_ERROR_UNDERFLOW(from, to, location, line) \
  SPECTRUM_ERROR_FLOW("underflow", from, to, location, line)

#define SPECTRUM_ERROR_OVERFLOW(from, to, location, line) \
  SPECTRUM_ERROR_FLOW("overflow", from, to, location, line)

/**
 * SPECTRUM_UNREACHABLE
 */
#define SPECTRUM_UNREACHABLE_STRING(name, string) \
  SPECTRUM_ERROR_FORMAT(name, "%s", string.c_str())
