// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include "Utils.h"

#include <spectrum/core/utils/Endianness.h>

#include <cstdint>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {

template <typename T>
struct RationalBase : public core::utils::CustomByteConvertible {
  using _Value = T;

  T numerator;
  T denominator;

  RationalBase(const T numerator, const T denominator)
      : numerator(numerator), denominator(denominator) {}

  double value() const {
    return static_cast<double>(numerator) / static_cast<double>(denominator);
  }
} __attribute__((packed));

typedef RationalBase<std::uint32_t> Rational;
typedef RationalBase<int32_t> SRational;

static_assert(
    sizeof(Rational) == 8,
    "Unexpected Rational size... Did something change?");
static_assert(
    sizeof(SRational) == 8,
    "Unexpected SRational size... Did something change?");

template <typename T>
bool operator==(const RationalBase<T>& lhs, const RationalBase<T>& rhs) {
  return lhs.numerator == rhs.numerator && lhs.denominator == rhs.denominator;
}

} // namespace metadata
} // namespace image

namespace core {
namespace utils {

template <typename B, typename T = image::metadata::RationalBase<B>>
image::metadata::RationalBase<B> swapValueByteOrder(
    image::metadata::RationalBase<B> value) {
  return T{swapValueByteOrder(value.numerator),
           swapValueByteOrder(value.denominator)};
}

} // namespace utils
} // namespace core

} // namespace spectrum
} // namespace facebook
