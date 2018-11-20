// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <cstddef>
#include <cstdint>

namespace facebook {
namespace spectrum {
namespace core {

struct DataRange {
  DataRange(
      const std::uint8_t* const begin = nullptr,
      const std::size_t length = 0)
      : begin(begin), length(length) {}

  const std::uint8_t* begin;
  std::size_t length;

  const std::uint8_t* end() const {
    return begin + length;
  }

  const bool valid() const {
    return begin != nullptr;
  }
};

} // namespace core
} // namespace spectrum
} // namespace facebook
