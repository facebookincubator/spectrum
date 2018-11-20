// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Utils.h"

#include <cstring>

#include <spectrum/core/SpectrumEnforce.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace utils {

std::uint32_t make4ByteDataFromValue(const std::vector<std::uint8_t>& values) {
  SPECTRUM_ENFORCE_IF_NOT(values.size() <= sizeof(std::uint32_t));
  std::uint32_t value = 0;
  std::memcpy(&value, values.data(), values.size());
  return value;
}

} // namespace utils
} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
