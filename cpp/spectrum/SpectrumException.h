// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <folly/Optional.h>
#include <folly/Range.h>

namespace facebook {
namespace spectrum {

namespace error {
extern const folly::StringPiece Unknown;
} // namespace error

class SpectrumException : public std::exception {
 public:
  /** Name that groups similar exceptions together (e.g. all compressor
   * failures) */
  const std::string name;

  /** The specific error message (e.g. input forms no valid JPEG stream) */
  const folly::Optional<std::string> message;

  /** The location of the error including method and line number */
  const std::string location;

  /** Additional verbose descriptions (e.g. the functions parameters and state)
   */
  const std::string description;

  SpectrumException(
      const folly::StringPiece& name,
      const folly::Optional<std::string>& message,
      const char* const function,
      const unsigned int line);

  virtual const char* what() const noexcept override;
};

} // namespace spectrum
} // namespace facebook
