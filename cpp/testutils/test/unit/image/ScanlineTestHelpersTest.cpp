// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/Scanline.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <memory>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace testutils {
namespace test {

//
// makeScanline
//

TEST(image_testutils_ProcTestHelper, makeScanline_whenEmptyList_thenEmpty) {
  auto scanline = makeScanlineRgb({});
  ASSERT_EQ(0, scanline->sizeBytes());
}

TEST(
    image_testutils_ProcTestHelper,
    makeScanline_givenElements_thenScanlineContainsElements) {
  auto scanline = makeScanlineGray({{2}, {3}, {5}, {7}});
  ASSERT_EQ(4, scanline->width());
  ASSERT_EQ(4, scanline->sizeBytes());

  const auto expectedBytes = std::vector<std::uint8_t>{2, 3, 5, 7};

  ASSERT_EQ(
      0,
      std::memcmp(
          expectedBytes.data(), scanline->data(), expectedBytes.size()));
}

//
// assertScanline
//

TEST(
    image_testutils_ProcTestHelper,
    assertScanline_whenGivenNullPtrScanline_thenFail) {
  ASSERT_FALSE(assertScanlineGray({}, nullptr));
}

TEST(
    image_testutils_ProcTestHelper,
    assertScanline_whenGivenEmptyScanlineAndNoElementes_thenSucceed) {
  ASSERT_TRUE(assertScanlineGray({}, makeScanlineGray({}).get()));
}

TEST(
    image_testutils_ProcTestHelper,
    assertScanline_whenGivenEmptyScanlineButElementes_thenFail) {
  ASSERT_FALSE(assertScanlineGray({{2}, {5}}, makeScanlineGray({}).get()));
}

TEST(
    image_testutils_ProcTestHelper,
    assertScanline_whenGivenNonEmptyScanlineButNoElementes_thenFail) {
  ASSERT_FALSE(assertScanlineGray({}, makeScanlineGray({{5}, {7}}).get()));
}

TEST(
    image_testutils_ProcTestHelper,
    assertScanline_whenGivenNonEmptyScanlineAndMatchingElementes_thenSucceed) {
  ASSERT_TRUE(
      assertScanlineGray({{5}, {7}}, makeScanlineGray({{5}, {7}}).get()));
}

TEST(
    image_testutils_ProcTestHelper,
    assertScanline_whenGivenNonEmptyScanlineAndNonMatchingElementes_thenFail) {
  ASSERT_FALSE(assertScanlineGray(
      {{5}, {7}, {8}, {9}}, makeScanlineGray({{5}, {7}, {7}, {9}}).get()));
}

//
// Gray8 specialisations
//

TEST(image_testutils_ProcTestHelper, assertScanline_whenSame_thenSucceed) {
  ASSERT_TRUE(
      assertScanlineGray({{5}, {7}}, makeScanlineGray({{5}, {7}}).get()));
}

TEST(image_testutils_ProcTestHelper, assertScanline_whenDifferent_thenFail) {
  ASSERT_FALSE(
      assertScanlineGray({{5}, {7}}, makeScanlineGray({{5}, {6}}).get()));
}

} // namespace test
} // namespace helper
} // namespace proc
} // namespace spectrum
} // namespace facebook
