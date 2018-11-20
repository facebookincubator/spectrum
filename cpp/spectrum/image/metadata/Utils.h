// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace utils {

/**
 * Creates a new vector of bytes representing the passed value.
 *
 * @param value to be converted.
 * @return an byte-array of the value.
 */
template <typename T>
std::vector<std::uint8_t> makeDataFromValue(const T& value) {
  static_assert(
      std::is_same<T, bool>::value == false, "Cannot create data from bool");

  const auto valueAddress = reinterpret_cast<const std::uint8_t*>(&value);
  return std::vector<std::uint8_t>(valueAddress, valueAddress + sizeof(value));
}

/**
 * Inserts a vector of bytes representing the passed value.
 *
 * @param value to be inserted.
 * @param data to array to insert the value into.
 */
template <typename T>
void insertValueIntoData(const T& value, std::vector<std::uint8_t>& data) {
  static_assert(
      std::is_same<T, bool>::value == false, "Cannot create data from bool");

  const auto valueAddress = reinterpret_cast<const std::uint8_t*>(&value);
  data.insert(data.end(), valueAddress, valueAddress + sizeof(value));
}

/**
 * Creates a new vector of bytes representing the passed array of values.
 *
 * @param values an array of values to be converted.
 * @return an byte-array of the consecutive values.
 */
template <typename T>
std::vector<std::uint8_t> makeDataFromValue(const std::vector<T>& values) {
  static_assert(
      std::is_same<T, bool>::value == false, "Cannot create data from bool");

  const auto valuesDataAddress =
      reinterpret_cast<const std::uint8_t*>(values.data());
  return std::vector<std::uint8_t>(
      valuesDataAddress, valuesDataAddress + (values.size() * sizeof(T)));
}

/**
 * Creates an 4-byte unsigned integer from the passed array of bytes.
 *
 * @param values the array of bytes to convert to a 4-byte word.
 * @return the 4-byte representation of the array of bytes.
 */
std::uint32_t make4ByteDataFromValue(const std::vector<std::uint8_t>& values);

} // namespace utils
} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
