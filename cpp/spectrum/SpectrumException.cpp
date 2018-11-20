// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "SpectrumException.h"

#include <folly/Range.h>

#include <sstream>

namespace facebook {
namespace spectrum {

namespace error {
const folly::StringPiece Unknown{"unknown"};
} // namespace error

namespace {
std::string descriptionStringFromValues(
    const std::string& name,
    const folly::Optional<std::string>& message,
    const std::string& location) {
  std::string string = name + "#";

  if (message.hasValue()) {
    string += *message;
  }

  string += "-" + location;
  return string;
}

std::string makeLocation(
    const char* const function,
    const unsigned int line) {
  std::stringstream ss;
  ss << function << ":" << line;
  return ss.str();
}
} // namespace

SpectrumException::SpectrumException(
    const folly::StringPiece& name,
    const folly::Optional<std::string>& message,
    const char* const function,
    const unsigned int line)
    : name(name),
      message(message),
      location(makeLocation(function, line)),
      description(descriptionStringFromValues(
          this->name,
          this->message,
          this->location)) {}

const char* SpectrumException::what() const noexcept {
  return description.c_str();
}
} // namespace spectrum
} // namespace facebook
