// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/io/VectorImageSource.h>
#include <spectrum/testutils/TestUtils.h>

#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace io {
namespace test {

TEST(VectorImageSource, whenConstructedFromEmpty_thenNothingRead) {
  auto imageSource = io::testutils::makeVectorImageSource("");

  testutils::assertRead("", 10, imageSource);
  testutils::assertRead("", 0, imageSource);
  testutils::assertRead("", 1, imageSource);
}

TEST(VectorImageSource, whenConstructedFromString_thenStringRead) {
  auto imageSource =
      io::testutils::makeVectorImageSource("abcdefghijklmnopqrstuvwxzy");

  testutils::assertRead("abcdefghijklmnopqrst", 20, imageSource);
  testutils::assertRead("", 0, imageSource);
  testutils::assertRead("uvwxzy", 20, imageSource);
  testutils::assertRead("", 20, imageSource);
}

TEST(VectorImageSource, whenReadFromString_thenByteCountCorrect) {
  auto imageSource =
      io::testutils::makeVectorImageSource("abcdefghijklmnopqrst");

  testutils::assertRead("abcdefghij", 10, imageSource);
  ASSERT_EQ(10, imageSource.getTotalBytesRead());

  testutils::assertRead("", 0, imageSource);
  ASSERT_EQ(10, imageSource.getTotalBytesRead());

  testutils::assertRead("klmnopqrst", 10, imageSource);
  ASSERT_EQ(20, imageSource.getTotalBytesRead());

  testutils::assertRead("", 20, imageSource);
  ASSERT_EQ(20, imageSource.getTotalBytesRead());
}

TEST(
    VectorBitmapImageSource,
    whenConstructedWithSpecification_thenSpecificationReads) {
  const auto specification = image::testutils::makeDummyImageSpecification();
  auto imageSource = io::VectorBitmapImageSource({}, specification);
  ASSERT_EQ(specification, imageSource.imageSpecification());
}

} // namespace test
} // namespace io
} // namespace spectrum
} // namespace facebook
