// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Resize.h"

namespace facebook {
namespace spectrum {
namespace requirements {

Resize::Mode Resize::makeModeFromValue(const std::uint32_t value) {
  SPECTRUM_ENFORCE_IF_NOT(
      value >= static_cast<std::uint32_t>(Resize::Mode::Exact) &&
      value <= static_cast<std::uint32_t>(Resize::Mode::ExactOrLarger));
  return static_cast<Resize::Mode>(value);
}

bool Resize::operator==(const Resize& rhs) const {
  return mode == rhs.mode && targetSize == rhs.targetSize;
}

bool Resize::operator!=(const Resize& rhs) const {
  return !(*this == rhs);
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
