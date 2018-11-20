// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "SpectrumEnforce.h"

#include <folly/Optional.h>

#include <sstream>
#include <stdexcept>
#include <string>

namespace facebook {
namespace spectrum {
namespace core {

namespace {
folly::Optional<std::string> formatString(
    const char* const format,
    va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);

  char buffer[512];
  const auto requiredSize = vsnprintf(buffer, sizeof(buffer), format, args);
  folly::Optional<std::string> result;

  if (requiredSize > 0) {
    if (requiredSize < sizeof(buffer)) {
      result = std::string(buffer);
    } else {
      char longBuffer[requiredSize + 1];
      vsnprintf(longBuffer, sizeof(longBuffer), format, args_copy);
      result = std::string(longBuffer);
    }
  }

  va_end(args_copy);
  return result;
}
} // namespace

void internalThrowError(
    const char* const function,
    const std::uint32_t line,
    const folly::StringPiece& name,
    const char* const extraFormat,
    ...) {
  va_list args;
  va_start(args, extraFormat);
  const auto formattedExtra = formatString(extraFormat, args);
  va_end(args);

  throw facebook::spectrum::SpectrumException(
      name, formattedExtra, function, line);
}

} // namespace error
} // namespace spectrum
} // namespace facebook
