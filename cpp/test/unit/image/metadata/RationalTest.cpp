// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/metadata/Rational.h>
#include <spectrum/image/metadata/Utils.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace test {

TEST(image_metadata_Rational, whenByteSwapped_thenCorrectValueReturned) {
  const auto rational = Rational{1, 2};
  const auto swappedRational = core::utils::swapValueByteOrder(rational);

  ASSERT_EQ(
      core::utils::swapValueByteOrder(rational.numerator),
      swappedRational.numerator);
  ASSERT_EQ(
      core::utils::swapValueByteOrder(rational.denominator),
      swappedRational.denominator);
}

} // namespace test
} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
