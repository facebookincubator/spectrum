// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/proc/ScalingScanlineProcessingBlock.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <memory>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {
namespace test {

//
// MagicKernel
//

TEST(ScalingScanlineProcessingBlock, magic_whenNoScale_thenEqual) {
  const image::Size inputSize = {3, 3};
  const image::Size outputSize = {3, 3};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::MagicKernel);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));
  block.consume(image::testutils::makeScanlineGray({{7}, {8}, {9}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{1}, {2}, {3}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{4}, {5}, {6}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{7}, {8}, {9}}, block.produce().get()));
}

TEST(ScalingScanlineProcessingBlock, magic_whenReadingAfterFinished_thenNull) {
  const image::Size inputSize = {3, 3};
  const image::Size outputSize = {4, 4};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::MagicKernel);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));
  block.consume(image::testutils::makeScanlineGray({{7}, {8}, {9}}));

  ASSERT_TRUE(block.produce());
  ASSERT_TRUE(block.produce());
  ASSERT_TRUE(block.produce());
  ASSERT_TRUE(block.produce());

  ASSERT_FALSE(block.produce());
  ASSERT_FALSE(block.produce());
  ASSERT_FALSE(block.produce());
}

TEST(ScalingScanlineProcessingBlock, magic_whenReadingBeforeFinished_thenNull) {
  const image::Size inputSize = {3, 3};
  const image::Size outputSize = {2, 2};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::MagicKernel);

  ASSERT_FALSE(block.produce());
  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  ASSERT_FALSE(block.produce());
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));
  ASSERT_FALSE(block.produce());
  block.consume(image::testutils::makeScanlineGray({{7}, {8}, {9}}));

  ASSERT_TRUE(block.produce());
  ASSERT_TRUE(block.produce());
  ASSERT_FALSE(block.produce());
}

TEST(
    ScalingScanlineProcessingBlock,
    magic_whenDownscaleUniformImage_thenEqual) {
  const image::Size inputSize = {4, 4};
  const image::Size outputSize = {3, 3};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::MagicKernel);

  block.consume(
      image::testutils::makeScanlineGray({{100}, {100}, {100}, {100}}));
  block.consume(
      image::testutils::makeScanlineGray({{100}, {100}, {100}, {100}}));
  block.consume(
      image::testutils::makeScanlineGray({{100}, {100}, {100}, {100}}));
  block.consume(
      image::testutils::makeScanlineGray({{100}, {100}, {100}, {100}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{100}, {100}, {100}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{100}, {100}, {100}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{100}, {100}, {100}}, block.produce().get()));
  ASSERT_FALSE(block.produce());
}

TEST(
    ScalingScanlineProcessingBlock,
    magic_whenDownscaleUniformImageRgb_thenEqual) {
  const image::Size inputSize = {4, 4};
  const image::Size outputSize = {3, 3};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::RGB,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::MagicKernel);

  block.consume(image::testutils::makeScanlineRgb(
      {{32, 64, 128}, {32, 64, 128}, {32, 64, 128}, {32, 64, 128}}));
  block.consume(image::testutils::makeScanlineRgb(
      {{32, 64, 128}, {32, 64, 128}, {32, 64, 128}, {32, 64, 128}}));
  block.consume(image::testutils::makeScanlineRgb(
      {{32, 64, 128}, {32, 64, 128}, {32, 64, 128}, {32, 64, 128}}));
  block.consume(image::testutils::makeScanlineRgb(
      {{32, 64, 128}, {32, 64, 128}, {32, 64, 128}, {32, 64, 128}}));

  ASSERT_TRUE(image::testutils::assertScanlineRgb(
      {{32, 64, 128}, {32, 64, 128}, {32, 64, 128}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineRgb(
      {{32, 64, 128}, {32, 64, 128}, {32, 64, 128}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineRgb(
      {{32, 64, 128}, {32, 64, 128}, {32, 64, 128}}, block.produce().get()));
  ASSERT_FALSE(block.produce());
}

//
// Bicubic
//

TEST(ScalingScanlineProcessingBlock, bicubic_whenNoScale_thenEqual) {
  const image::Size inputSize = {3, 3};
  const image::Size outputSize = {3, 3};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::Bicubic);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));
  block.consume(image::testutils::makeScanlineGray({{7}, {8}, {9}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{1}, {2}, {3}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{4}, {5}, {6}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{7}, {8}, {9}}, block.produce().get()));
}

TEST(
    ScalingScanlineProcessingBlock,
    bicubic_whenReadingAfterFinished_thenNull) {
  const image::Size inputSize = {3, 3};
  const image::Size outputSize = {3, 3};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::Bicubic);

  block.consume(image::testutils::makeScanlineGray({{1}, {2}, {3}}));
  block.consume(image::testutils::makeScanlineGray({{4}, {5}, {6}}));
  block.consume(image::testutils::makeScanlineGray({{7}, {8}, {9}}));

  ASSERT_TRUE(block.produce());
  ASSERT_TRUE(block.produce());
  ASSERT_TRUE(block.produce());

  ASSERT_FALSE(block.produce());
  ASSERT_FALSE(block.produce());
  ASSERT_FALSE(block.produce());
}

TEST(
    ScalingScanlineProcessingBlock,
    bicubic_whenDownscaleUniformImage_thenEqual) {
  const image::Size inputSize = {5, 5};
  const image::Size outputSize = {3, 3};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::Bicubic);

  block.consume(image::testutils::makeScanlineGray({{1}, {1}, {1}, {1}, {1}}));
  block.consume(image::testutils::makeScanlineGray({{1}, {1}, {1}, {1}, {1}}));
  block.consume(image::testutils::makeScanlineGray({{1}, {1}, {1}, {1}, {1}}));
  block.consume(image::testutils::makeScanlineGray({{1}, {1}, {1}, {1}, {1}}));
  block.consume(image::testutils::makeScanlineGray({{1}, {1}, {1}, {1}, {1}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{1}, {1}, {1}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{1}, {1}, {1}}, block.produce().get()));
}

TEST(ScalingScanlineProcessingBlock, bicubic_whenDownscaleAllBlack_thenEqual) {
  const image::Size inputSize = {3, 3};
  const image::Size outputSize = {2, 2};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::Bicubic);

  block.consume(image::testutils::makeScanlineGray({{0}, {0}, {0}}));
  block.consume(image::testutils::makeScanlineGray({{0}, {0}, {0}}));
  block.consume(image::testutils::makeScanlineGray({{0}, {0}, {0}}));

  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{0}, {0}}, block.produce().get()));
  ASSERT_TRUE(
      image::testutils::assertScanlineGray({{0}, {0}}, block.produce().get()));
}

TEST(ScalingScanlineProcessingBlock, bicubic_whenDownscaleAllWhite_thenEqual) {
  const image::Size inputSize = {3, 3};
  const image::Size outputSize = {2, 2};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::Gray,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::Bicubic);

  block.consume(image::testutils::makeScanlineGray({{255}, {255}, {255}}));
  block.consume(image::testutils::makeScanlineGray({{255}, {255}, {255}}));
  block.consume(image::testutils::makeScanlineGray({{255}, {255}, {255}}));

  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{255}, {255}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineGray(
      {{255}, {255}}, block.produce().get()));
}

TEST(ScalingScanlineProcessingBlock, whenNoScale_thenEqualRbg) {
  const image::Size inputSize = {1, 3};
  const image::Size outputSize = {1, 3};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::RGB,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::Bicubic);

  block.consume(
      image::testutils::makeScanlineRgb({{1, 1, 5}, {3, 3, 3}, {5, 5, 1}}));

  ASSERT_TRUE(image::testutils::assertScanlineRgb(
      {{1, 1, 5}, {3, 3, 3}, {5, 5, 1}}, block.produce().get()));
}

TEST(ScalingScanlineProcessingBlock, whenDownscaleAllWhite_thenEqualRgb) {
  const image::Size inputSize = {5, 5};
  const image::Size outputSize = {3, 3};
  ScalingScanlineProcessingBlock block(
      image::pixel::specifications::RGB,
      inputSize,
      outputSize,
      Configuration::General::SamplingMethod::Bicubic);

  block.consume(image::testutils::makeScanlineRgb({{100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80}}));
  block.consume(image::testutils::makeScanlineRgb({{100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80}}));
  block.consume(image::testutils::makeScanlineRgb({{100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80}}));
  block.consume(image::testutils::makeScanlineRgb({{100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80}}));
  block.consume(image::testutils::makeScanlineRgb({{100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80},
                                                   {100, 90, 80}}));

  ASSERT_TRUE(image::testutils::assertScanlineRgb(
      {{100, 90, 80}, {100, 90, 80}, {100, 90, 80}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineRgb(
      {{100, 90, 80}, {100, 90, 80}, {100, 90, 80}}, block.produce().get()));
  ASSERT_TRUE(image::testutils::assertScanlineRgb(
      {{100, 90, 80}, {100, 90, 80}, {100, 90, 80}}, block.produce().get()));
}

} // namespace test
} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
