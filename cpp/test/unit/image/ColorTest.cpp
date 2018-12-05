// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/Color.h>

#include <gtest/gtest.h>

using namespace facebook::spectrum::image;

namespace facebook {
namespace spectrum {
namespace image {
namespace test {

TEST(image_Color, whenIdenticalCompared_thenEqual) {
  const auto a = Color{10, 20, 30};
  const auto b = Color{10, 20, 30};

  ASSERT_EQ(a, b);
}

TEST(image_Color, whenDifferentCompared_thenNotEqual) {
  const auto a = Color{10, 20, 30};
  const auto b = Color{20, 30, 40};

  ASSERT_NE(a, b);
}

} // namespace test
} // namespace image
} // namespace spectrum
} // namespace facebook
