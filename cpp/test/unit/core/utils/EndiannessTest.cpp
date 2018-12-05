// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/utils/Endianness.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace utils {
namespace test {

TEST(core_Utils, whenSwappingByteOrderOfValue_thenResultIsCorrect) {
  const auto value = std::uint32_t{0xA1B2C3D4};
  const auto swappedValue = std::uint32_t{0xD4C3B2A1};

  ASSERT_EQ(swappedValue, swapValueByteOrder(value));
  ASSERT_EQ(value, swapValueByteOrder(swappedValue));
}

TEST(core_Utils, whenConvertingToNativeByteOrder_thenResultCorrect) {
  const auto value = std::uint32_t{0xA1B2C3D4};
  const auto swappedValue = std::uint32_t{0xD4C3B2A1};

  ASSERT_EQ('X', convertValueToNativeByteOrder('X', true));
  ASSERT_EQ('X', convertValueToNativeByteOrder('X', false));

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  ASSERT_EQ(value, convertValueToNativeByteOrder(value, true));
  ASSERT_EQ(swappedValue, convertValueToNativeByteOrder(value, false));
#else
  ASSERT_EQ(swappedValue, convertValueToNativeEndian(value, true));
  ASSERT_EQ(value, convertValueToNativeEndian(value, false));
#endif
}

} // namespace test
} // namespace utils
} // namespace core
} // namespace spectrum
} // namespace facebook
