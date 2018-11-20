// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Rotate.h"

#include <spectrum/core/utils/Numeric.h>

#include <sstream>

namespace facebook {
namespace spectrum {
namespace requirements {

int Rotate::sanitisedDegrees() const {
  return core::numeric::angleDegreesSanitized(degrees);
}

bool Rotate::noop() const {
  return sanitisedDegrees() == 0 && !flipHorizontally && !flipVertically &&
      !forceUpOrientation;
}

bool Rotate::operator==(const Rotate& rhs) const {
  return degrees == rhs.degrees && flipVertically == rhs.flipVertically &&
      flipHorizontally == rhs.flipHorizontally &&
      forceUpOrientation == rhs.forceUpOrientation;
}

bool Rotate::flip() const {
  return flipVertically || flipHorizontally;
}

bool Rotate::operator!=(const Rotate& rhs) const {
  return !(*this == rhs);
}

std::string Rotate::string() const {
  std::stringstream ss;
  ss << "{";
  ss << "degrees:" << degrees << ",";
  ss << "flipVertically:" << flipVertically << ",";
  ss << "flipHorizontally:" << flipHorizontally << ",";
  ss << "forceUpOrientation:" << forceUpOrientation << "}";
  return ss.str();
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
