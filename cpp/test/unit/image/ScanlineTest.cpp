// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/Scanline.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::image;

namespace facebook {
namespace spectrum {
namespace image {
namespace test {

TEST(image_Scanline, whenSize_thenSizeInBytes) {
  auto scanline = Scanline(pixel::specifications::RGB, 10);
  ASSERT_EQ(30, scanline.sizeBytes());
}

TEST(image_Scanline, whenGetPixelSpecification_thenPixelSpecification) {
  auto scanline = Scanline(pixel::specifications::ARGB, 10);
  ASSERT_EQ(pixel::specifications::ARGB, scanline.specification());
}

TEST(image_Scanline, whenWritingData_thenDataRetrievable) {
  auto scanline = Scanline(pixel::specifications::RGB, 10);

  for (std::uint8_t offset = 0; offset < scanline.sizeBytes(); offset++) {
    *(scanline.data() + offset) = offset;
  }

  for (std::uint8_t offset = 0; offset < scanline.sizeBytes(); offset++) {
    ASSERT_EQ(offset, *(scanline.data() + offset));
  }
}

TEST(image_Scanline, copyPixelFromTo_whenCopyingRgb_thenMatches) {
  auto input = Scanline(pixel::specifications::RGB, 3);

  const auto pixelsData = std::vector<std::uint8_t>{
      0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

  std::memcpy(input.data(), pixelsData.data(), pixelsData.size());

  auto output = Scanline(pixel::specifications::RGB, 2);
  copyPixelFromTo(input, 0, output, 0);
  copyPixelFromTo(input, 2, output, 1);

  ASSERT_TRUE(std::memcmp(output.data(), pixelsData.data(), 3) == 0);
  ASSERT_TRUE(std::memcmp(output.data() + 3, pixelsData.data() + 6, 3) == 0);
}

} // namespace test
} // namespace image
} // namespace spectrum
} // namespace facebook
