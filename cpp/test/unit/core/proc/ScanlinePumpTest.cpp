// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/proc/ScanlinePump.h>
#include <spectrum/core/proc/RotationScanlineProcessingBlock.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {
namespace test {

TEST(ScanlinePump, whenConstructed_thenNoObservableEffects) {
  ScanlinePump scanlinePump(
      []() { return nullptr; },
      {},
      [](std::unique_ptr<image::Scanline> /* unused */) {},
      13);
}

TEST(ScanlinePump, whenNoProcessingBlocks_thenOutputEqualsInput) {
  std::vector<std::unique_ptr<image::Scanline>> output;
  ScanlinePump scanlinePump(
      []() {
        return image::testutils::makeScanlineGray({{1}, {2}, {3}, {4}, {5}});
      },
      {},
      [&](std::unique_ptr<image::Scanline> scanline) {
        output.push_back(std::move(scanline));
      },
      10);

  scanlinePump.pumpAll();

  ASSERT_EQ(10, output.size());
  for (std::size_t i = 0; i < 10; i++) {
    ASSERT_TRUE(image::testutils::assertScanlineGray(
        {{1}, {2}, {3}, {4}, {5}}, output[i].get()));
  }
}

TEST(ScanlinePump, whenOneRotationBlockOf90Degree_thenOutputRotated90Degree) {
  std::vector<std::unique_ptr<image::Scanline>> output;

  std::vector<std::unique_ptr<ScanlineProcessingBlock>> processingBlocks;
  processingBlocks.push_back(std::make_unique<RotationScanlineProcessingBlock>(
      image::pixel::specifications::Gray,
      image::Size{3, 4},
      image::Orientation::Right));

  ScanlinePump scanlinePump(
      []() {
        return image::testutils::makeScanlineGray({{1}, {2}, {3}});
      },
      std::move(processingBlocks),
      [&](std::unique_ptr<image::Scanline> scanline) {
        output.push_back(std::move(scanline));
      },
      4);

  scanlinePump.pumpAll();

  ASSERT_EQ(3, output.size());
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{1}, {1}, {1}, {1}}, output[0].get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{2}, {2}, {2}, {2}}, output[1].get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{3}, {3}, {3}, {3}}, output[2].get()));
}

TEST(ScanlinePump, whenFourRotationBlockOf90Degree_thenOutputRotated360Degree) {
  std::vector<std::unique_ptr<image::Scanline>> output;

  std::vector<std::unique_ptr<ScanlineProcessingBlock>> processingBlocks;
  processingBlocks.push_back(std::make_unique<RotationScanlineProcessingBlock>(
      image::pixel::specifications::Gray,
      image::Size{3, 4},
      image::Orientation::Right));
  processingBlocks.push_back(std::make_unique<RotationScanlineProcessingBlock>(
      image::pixel::specifications::Gray,
      image::Size{4, 3},
      image::Orientation::Right));
  processingBlocks.push_back(std::make_unique<RotationScanlineProcessingBlock>(
      image::pixel::specifications::Gray,
      image::Size{3, 4},
      image::Orientation::Right));
  processingBlocks.push_back(std::make_unique<RotationScanlineProcessingBlock>(
      image::pixel::specifications::Gray,
      image::Size{4, 3},
      image::Orientation::Right));

  ScanlinePump scanlinePump(
      []() {
        return image::testutils::makeScanlineGray({{1}, {2}, {3}});
      },
      std::move(processingBlocks),
      [&](std::unique_ptr<image::Scanline> scanline) {
        output.push_back(std::move(scanline));
      },
      4);

  scanlinePump.pumpAll();

  // 360 degree == 0 degree
  ASSERT_EQ(4, output.size());
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{1}, {2}, {3}}, output[0].get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{1}, {2}, {3}}, output[1].get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{1}, {2}, {3}}, output[2].get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{1}, {2}, {3}}, output[3].get()));
}

} // namespace test
} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
