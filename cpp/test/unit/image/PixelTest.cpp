// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/Pixel.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace pixel {
namespace test {

TEST(
    image_pixel_ColorModel,
    whenCommonColorModel_thenHasExpectedColorComponents) {
  ASSERT_EQ(1, colormodels::Gray.numberOfColorComponents);
  ASSERT_EQ(3, colormodels::RGB.numberOfColorComponents);
  ASSERT_EQ(4, colormodels::CMYK.numberOfColorComponents);
  ASSERT_EQ(3, colormodels::YCbCr.numberOfColorComponents);
  ASSERT_EQ(4, colormodels::YCbCrK.numberOfColorComponents);
}

TEST(image_pixel_AlphaInfo, whenCreatedFromValue_thenMatches) {
  ASSERT_EQ(
      AlphaInfo::None, alphaInfoFromValue(static_cast<int>(AlphaInfo::None)));
  ASSERT_EQ(
      AlphaInfo::PremultipliedLast,
      alphaInfoFromValue(static_cast<int>(AlphaInfo::PremultipliedLast)));
}

TEST(image_pixel_AlphaInfo, whenCreatedFromValueOutsideRange_thenThrows) {
  ASSERT_ANY_THROW(alphaInfoFromValue(-1));
  ASSERT_ANY_THROW(
      alphaInfoFromValue(static_cast<int>(AlphaInfo::PremultipliedLast) + 1));
}

TEST(image_pixel_ComponentsOrder, whenCreatedFromValue_thenMatches) {
  ASSERT_EQ(
      ComponentsOrder::Natural,
      makeComponentsOrderFromValue(static_cast<int>(ComponentsOrder::Natural)));
  ASSERT_EQ(
      ComponentsOrder::Reversed,
      makeComponentsOrderFromValue(
          static_cast<int>(ComponentsOrder::Reversed)));
}

TEST(image_pixel_ComponentsOrder, whenCreatedFromValueOutsideRange_thenThrows) {
  ASSERT_ANY_THROW(makeComponentsOrderFromValue(-1));
  ASSERT_ANY_THROW(makeComponentsOrderFromValue(
      static_cast<int>(ComponentsOrder::Reversed) + 1));
}

TEST(image_pixel_Specification, whenRGBA_thenMatchesCorrectBitsPerPixel) {
  ASSERT_EQ(4, specifications::RGBA.bytesPerPixel);
}

TEST(image_pixel_Specification, whenRGBA_thenMatchesCorrectComponentCount) {
  ASSERT_EQ(4, specifications::RGBA.numberOfComponents());
}

TEST(image_pixel_Specification, whenRGBA_thenHasAlpha) {
  ASSERT_EQ(true, specifications::RGBA.hasAlpha());
}

TEST(image_pixel_Specification, whenIdentical_thenEqual) {
  ASSERT_EQ(specifications::RGB, specifications::RGB);
}

TEST(image_pixel_Specification, whenDifferInAlpha_thenNotEqual) {
  ASSERT_NE(specifications::Gray, specifications::GrayA);
}

TEST(image_pixel_Specification, whenDifferInOrder_thenNotEqual) {
  ASSERT_NE(specifications::RGBA, specifications::ABGR);
}

} // namespace test
} // namespace pixel
} // namespace image
} // namespace spectrum
} // namespace facebook
