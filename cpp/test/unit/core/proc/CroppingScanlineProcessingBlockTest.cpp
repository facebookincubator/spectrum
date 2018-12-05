// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/proc/CroppingScanlineProcessingBlock.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <memory>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {
namespace test {

TEST(CroppingScanlineProcessingBlock, whenTargetTooWide_thenThrow) {
  ASSERT_THROW(
      CroppingScanlineProcessingBlock block(
          image::pixel::specifications::Gray, {4, 4}, {{2, 2}, {3, 2}}),
      SpectrumException);
}

TEST(CroppingScanlineProcessingBlock, whenTargetTooHigh_thenThrow) {
  ASSERT_THROW(
      CroppingScanlineProcessingBlock block(
          image::pixel::specifications::Gray, {4, 4}, {{2, 2}, {2, 3}}),
      SpectrumException);
}

TEST(CroppingScanlineProcessingBlock, whenTargetToWide_thenThrow) {
  ASSERT_THROW(
      CroppingScanlineProcessingBlock block(
          image::pixel::specifications::Gray,
          {4, 4},
          {{2, 2}, image::sizeZero}),
      SpectrumException);
}

TEST(
    CroppingScanlineProcessingBlock,
    whenTargetMatchesInput_thenOutputCorrect) {
  CroppingScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {4, 4}, {image::pointZero, {4, 4}});

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}, {4}}));
  block.consume(image::testutils::makeScanlineGray({{5}, {6}, {7}, {8}}));
  block.consume(image::testutils::makeScanlineGray({{9}, {10}, {11}, {12}}));
  block.consume(image::testutils::makeScanlineGray({{13}, {14}, {15}, {16}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{1}, {2}, {3}, {4}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{5}, {6}, {7}, {8}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{9}, {10}, {11}, {12}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{13}, {14}, {15}, {16}}, block.produce().get()));
  ASSERT_EQ(nullptr, block.produce());
}

TEST(
    CroppingScanlineProcessingBlock,
    whenTargetMatchesTopLeft_thenOutputCorrect) {
  CroppingScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {4, 4}, {image::pointZero, {2, 2}});

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}, {4}}));
  block.consume(image::testutils::makeScanlineGray({{5}, {6}, {7}, {8}}));
  block.consume(image::testutils::makeScanlineGray({{9}, {10}, {11}, {12}}));
  block.consume(image::testutils::makeScanlineGray({{13}, {14}, {15}, {16}}));

  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{1}, {2}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{5}, {6}}, block.produce().get()));
  ASSERT_EQ(nullptr, block.produce());
}

TEST(
    CroppingScanlineProcessingBlock,
    whenTargetMatchesTopRight_thenOutputCorrect) {
  CroppingScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {4, 4}, {{2, 0}, {2, 2}});

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}, {4}}));
  block.consume(image::testutils::makeScanlineGray({{5}, {6}, {7}, {8}}));
  block.consume(image::testutils::makeScanlineGray({{9}, {10}, {11}, {12}}));
  block.consume(image::testutils::makeScanlineGray({{13}, {14}, {15}, {16}}));

  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{3}, {4}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{7}, {8}}, block.produce().get()));
  ASSERT_EQ(nullptr, block.produce());
}

TEST(
    CroppingScanlineProcessingBlock,
    whenTargetMatchesBottomLeft_thenOutputCorrect) {
  CroppingScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {4, 4}, {{0, 2}, {2, 2}});

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}, {4}}));
  block.consume(image::testutils::makeScanlineGray({{5}, {6}, {7}, {8}}));
  block.consume(image::testutils::makeScanlineGray({{9}, {10}, {11}, {12}}));
  block.consume(image::testutils::makeScanlineGray({{13}, {14}, {15}, {16}}));

  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{9}, {10}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{13}, {14}}, block.produce().get()));
  ASSERT_EQ(nullptr, block.produce());
}

TEST(
    CroppingScanlineProcessingBlock,
    whenTargetMatchesBottomRight_thenOutputCorrect) {
  CroppingScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {4, 4}, {{2, 2}, {2, 2}});

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}, {4}}));
  block.consume(image::testutils::makeScanlineGray({{5}, {6}, {7}, {8}}));
  block.consume(image::testutils::makeScanlineGray({{9}, {10}, {11}, {12}}));
  block.consume(image::testutils::makeScanlineGray({{13}, {14}, {15}, {16}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{11}, {12}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{15}, {16}}, block.produce().get()));
  ASSERT_EQ(nullptr, block.produce());
}

TEST(
    CroppingScanlineProcessingBlock,
    whenTargetMatchesMiddle_thenNullOutputBeforeAndNullOutputAfter) {
  CroppingScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {4, 4}, {{1, 1}, {2, 2}});

  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}, {4}}));
  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());

  block.consume(image::testutils::makeScanlineGray({{5}, {6}, {7}, {8}}));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{6}, {7}}, block.produce().get()));
  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());

  block.consume(image::testutils::makeScanlineGray({{9}, {10}, {11}, {12}}));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{10}, {11}}, block.produce().get()));
  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());

  block.consume(image::testutils::makeScanlineGray({{13}, {14}, {15}, {16}}));
  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());
}

} // namespace test
} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
