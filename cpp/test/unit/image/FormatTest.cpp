// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/Format.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::image;

namespace facebook {
namespace spectrum {
namespace image {
namespace test {

TEST(image_Format, whenCreated_thenContainsIdentifier) {
  const auto a = Format{"a"};
  ASSERT_EQ("a", a.identifier());
}

TEST(image_EncodedFormat, whenCreated_thenContainsIdentifier) {
  const auto a = EncodedFormat{"a"};
  ASSERT_EQ("a", a.identifier());
}

TEST(image_EncodedFormat, whenComparingJpegJpeg_thenEqual) {
  ASSERT_EQ(formats::Jpeg, formats::Jpeg);
}

TEST(image_EncodedFormat, whenComparingJpegPng_thenNotEqual) {
  ASSERT_NE(formats::Bitmap, formats::Png);
}

TEST(image_EncodedFormat, whenComparingToFormat_thenNotEqual) {
  ASSERT_NE(formats::Jpeg, formats::Bitmap);
}

TEST(image_Format, whenBitmap_thenNotEncodedImageFormat) {
  ASSERT_FALSE(formats::Bitmap.encoded());
}

TEST(image_Format, whenJpeg_thenEncodedImageFormat) {
  ASSERT_TRUE(formats::Jpeg.encoded());
}

} // namespace test
} // namespace image
} // namespace spectrum
} // namespace facebook
