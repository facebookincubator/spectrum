// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/proc/RotationScanlineProcessingBlock.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <memory>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {
namespace test {

TEST(RotationScanlineProcessingBlock, whenRotateUp_thenThrow) {
  ASSERT_ANY_THROW(RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {3, 2}, image::Orientation::Up));
}

TEST(
    RotationScanlineProcessingBlock,
    whenRotateUpMirrored_thenScanlinesRotated) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      {3, 2},
      image::Orientation::UpMirrored);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{3}, {2}, {1}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{6}, {5}, {4}}, block.produce().get()));
}

TEST(RotationScanlineProcessingBlock, whenRotateRight_thenScanlinesRotated) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {3, 2}, image::Orientation::Right);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));

  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{4}, {1}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{5}, {2}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{6}, {3}}, block.produce().get()));
}

TEST(
    RotationScanlineProcessingBlock,
    whenRotateRightMirrored_thenScanlinesRotated) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      {3, 2},
      image::Orientation::RightMirrored);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));

  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{6}, {3}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{5}, {2}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{4}, {1}}, block.produce().get()));
}

TEST(RotationScanlineProcessingBlock, whenRotateBottom_thenScanlinesRotated) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {3, 2}, image::Orientation::Bottom);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{6}, {5}, {4}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{3}, {2}, {1}}, block.produce().get()));
}

TEST(
    RotationScanlineProcessingBlock,
    whenRotateBottomMirrored_thenScanlinesRotated) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      {3, 2},
      image::Orientation::BottomMirrored);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{4}, {5}, {6}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{1}, {2}, {3}}, block.produce().get()));
}

TEST(RotationScanlineProcessingBlock, whenRotateLeft_thenScanlinesRotated) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {3, 2}, image::Orientation::Left);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));

  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{3}, {6}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{2}, {5}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{1}, {4}}, block.produce().get()));
}

TEST(
    RotationScanlineProcessingBlock,
    whenRotateLeftMirrored_thenScanlinesRotated) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      {3, 2},
      image::Orientation::LeftMirrored);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));

  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{1}, {4}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{2}, {5}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{3}, {6}}, block.produce().get()));
}

TEST(
    RotationScanlineProcessingBlock,
    whenProduceCalledBeforeLastConsume_thenThrow) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {3, 2}, image::Orientation::Left);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));

  ASSERT_EQ(nullptr, block.produce());
}

TEST(
    RotationScanlineProcessingBlock,
    whenConsumeCalledWithScanlineOfWrongWidth_thenThrow) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {3, 2}, image::Orientation::Right);

  ASSERT_ANY_THROW(
      block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}, {4}})));
}

TEST(
    RotationScanlineProcessingBlock,
    whenCalledOnEmptyState_thenReturnNullPtrUntilWaiting) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {3, 2}, image::Orientation::Left);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));

  ASSERT_NE(nullptr, block.produce());
  ASSERT_NE(nullptr, block.produce());
  ASSERT_NE(nullptr, block.produce());
}

TEST(
    RotationScanlineProcessingBlock,
    whenProduceCalledMoreOftenThanRows_thenReturnNullptrFinally) {
  RotationScanlineProcessingBlock block(
      image::pixel::specifications::Gray, {3, 2}, image::Orientation::Left);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));

  ASSERT_NE(nullptr, block.produce());
  ASSERT_NE(nullptr, block.produce());
  ASSERT_NE(nullptr, block.produce());

  ASSERT_EQ(nullptr, block.produce());
  ASSERT_EQ(nullptr, block.produce());
}

} // namespace test
} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
