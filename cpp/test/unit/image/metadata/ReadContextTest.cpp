// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/metadata/ReadContext.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace test {

TEST(
    image_matadata_ReadContext,
    whenCreatingAContext_thenPropertiesAreCorrect) {
  // create array to ensure order of pointers
  const std::array<std::uint8_t, 2> data = {0, 0};
  const auto dataLength = std::size_t{42};
  const auto context = ReadContext{&data[0], dataLength, &data[1], false};
  ASSERT_EQ(&data[1], context.tiffHeaderBegin);
  ASSERT_EQ(&data[0] + dataLength, context.dataEnd);
  ASSERT_FALSE(context.isLittleEndianEncoded);
}

TEST(
    image_matadata_ReadContext,
    whenTiffAddressIsNotBetweenDataBeginAndDataEnd_thenThrows) {
  // create array to ensure order of pointers
  const std::array<std::uint8_t, 2> data = {0, 0};
  ASSERT_THROW(
      ReadContext(&data[1], std::size_t{42}, &data[0], false),
      SpectrumException);
}

TEST(image_matadata_ReadContext, whenNullptrIsPassedAsDataBegin_thenThrows) {
  const auto location = std::uint8_t{0};
  ASSERT_THROW(ReadContext(nullptr, 42, &location, false), SpectrumException);
}

TEST(
    image_matadata_ReadContext,
    whenNullptrIsPassedAsTiffHeaderBegin_thenThrows) {
  const auto location = std::uint8_t{0};
  ASSERT_THROW(ReadContext(&location, 42, nullptr, false), SpectrumException);
}

TEST(image_matadata_ReadContext, whenZeroIsPassedAsDataLength_thenThrows) {
  const auto location = std::uint8_t{0};
  const auto location2 = std::uint8_t{0};
  ASSERT_THROW(ReadContext(&location2, 0, &location, false), SpectrumException);
}

} // namespace test
} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
