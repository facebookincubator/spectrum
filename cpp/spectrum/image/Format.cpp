// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Format.h"

namespace facebook {
namespace spectrum {
namespace image {

const Format::Identifier& Format::identifier() const {
  return _identifier;
}

bool Format::encoded() const {
  return _encoded;
}

std::string Format::string() const {
  return std::string(_identifier);
}

bool Format::operator==(const Format& rhs) const {
  return _identifier == rhs._identifier && _encoded == rhs._encoded;
}

bool Format::operator!=(const Format& rhs) const {
  return !(*this == rhs);
}

} // namespace image
} // namespace spectrum
} // namespace facebook
