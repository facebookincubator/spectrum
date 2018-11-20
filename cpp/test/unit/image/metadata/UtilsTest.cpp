// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/metadata/Utils.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace utils {
namespace test {

TEST(
    image_metadata_Utils,
    whenCreatingDataFromPrimitiveValue_thenDataIsCorrect) {
  const auto value = std::uint32_t{0x8BADF00D};
  const auto valueData = makeDataFromValue(value);

  ASSERT_EQ(sizeof(std::uint32_t), valueData.size());
  ASSERT_EQ(value, *reinterpret_cast<const std::uint32_t*>(valueData.data()));
}

TEST(
    image_metadata_Utils,
    whenCreatingDataFromArrayOfPrimitiveValues_thenDataIsCorrect) {
  const auto values =
      std::vector<std::uint32_t>{0x8BADF00D, 0xBAAAAAAD, 0xDEADBAAD};
  const auto valuesData = makeDataFromValue(values);
  const auto valuesDataAddress =
      reinterpret_cast<const std::uint32_t*>(valuesData.data());

  ASSERT_EQ(sizeof(std::uint32_t) * values.size(), valuesData.size());
  ASSERT_EQ(values[0], valuesDataAddress[0]);
  ASSERT_EQ(values[1], valuesDataAddress[1]);
  ASSERT_EQ(values[2], valuesDataAddress[2]);
}

} // namespace test
} // namespace utils
} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
