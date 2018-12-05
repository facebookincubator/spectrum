// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/utils/Numeric.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::core::numeric;

namespace facebook {
namespace spectrum {
namespace core {
namespace numeric {
namespace test {
const auto lowestFloat = std::numeric_limits<float>::lowest();
const auto lowestInt = std::numeric_limits<int>::lowest();

template <typename In, typename Out>
Out _performConversion(const In& in) {
  return SPECTRUM_CONVERT_OR_THROW(in, Out);
}

TEST(Numeric, whenConvertingValuesOutOfLowerBound_thenThrows) {
  ASSERT_ANY_THROW((_performConversion<double, std::uint32_t>(-1.0)));
  ASSERT_ANY_THROW((_performConversion<float, std::uint32_t>(-1.0f)));
  ASSERT_ANY_THROW((_performConversion<int, std::uint32_t>(-1)));
  ASSERT_ANY_THROW((_performConversion<float, int>(lowestFloat)));
}

TEST(Numeric, whenConvertingValuesOutOfUpperBound_thenThrows) {
  ASSERT_ANY_THROW((_performConversion<std::uint32_t, std::uint8_t>(0xFF0)));
  ASSERT_ANY_THROW((_performConversion<double, std::uint32_t>(
      std::numeric_limits<double>::max())));
}

TEST(Numeric, whenConvertingValuesWithinfBounds_thenSucceed) {
  ASSERT_EQ(-1.0, (_performConversion<int, double>(-1)));
  ASSERT_EQ(-1.0, (_performConversion<int, float>(-1)));
  ASSERT_EQ(-1.0, (_performConversion<int, float>(-1)));
  ASSERT_EQ(-1.0, (_performConversion<int, float>(-1)));
  ASSERT_EQ(0, (_performConversion<float, std::uint32_t>(0.1)));
  ASSERT_EQ(0, (_performConversion<float, std::uint32_t>(0.1)));
  ASSERT_EQ(lowestInt, (_performConversion<float, int>(lowestInt)));
}

} // namespace test
} // namespace numeric
} // namespace core
} // namespace spectrum
} // namespace facebook
