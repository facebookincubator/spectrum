// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <algorithm>
#include <cstdint>
#include <type_traits>

#ifndef __BYTE_ORDER__
#error __BYTE_ORDER__ undefined!
#elif __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__ && \
    __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
#error Unexpected __BYTE_ORDER__ value!
#endif

namespace facebook {
namespace spectrum {
namespace core {
namespace utils {

/**
 * Marker interface that when subclassed, opts-out a class from having it bytes
 * swapped automatically. See `swapValueByteOrder`.
 */
class CustomByteConvertible {
  // No Data, marker interface.
};

/**
 * Swaps the byte-order of a value.
 *
 * @param value the value to toggle.
 * @return the swapped value.
 * @note if values are composite, they'll need to implement their own
 * `swapValueByteOrder` and inherit from `CustomByteConvertible.
 */
template <
    typename T,
    std::enable_if_t<!std::is_same<T, CustomByteConvertible>::value>* = nullptr>
T swapValueByteOrder(T value) noexcept {
  std::uint8_t* ptr = reinterpret_cast<std::uint8_t*>(&value);
  std::reverse(ptr, ptr + sizeof(T));
  return value;
}

/**
 * Converts a value to the platform's byte-order.
 *
 * @param value the value to convert.
 * @param isLittleEndianEncoded describes whether `value` is encoded in little
 * or big endian.
 * @return the value in the platform's byte-order.
 */
template <typename T>
T convertValueToNativeByteOrder(
    const T value,
    const bool isLittleEndianEncoded) noexcept {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  return isLittleEndianEncoded || sizeof(T) == 1 ? value
                                                 : swapValueByteOrder(value);
#else
  return isLittleEndianEncoded && sizeof(T) != 1 ? swapValueByteOrder(value)
                                                 : value;
#endif
}

} // namespace utils
} // namespace core
} // namespace spectrum
} // namespace facebook
