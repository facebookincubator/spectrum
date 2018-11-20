// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/proc/ScanlineProcessingBlock.h>
#include <spectrum/image/Geometry.h>
#include <spectrum/image/Scanline.h>

#include <memory>
#include <queue>
#include <stdexcept>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

/**
 * Reads scanlines from an input image and returns only pixels from within
 * the targetted area
 */
class CroppingScanlineProcessingBlock : public ScanlineProcessingBlock {
 private:
  const image::pixel::Specification _pixelSpecification;
  const image::Size inputSize;
  const image::Rect cropRect;

  std::queue<std::unique_ptr<image::Scanline>> output;
  std::size_t inputScanline = 0;

 public:
  CroppingScanlineProcessingBlock(
      const image::pixel::Specification& pixelSpecification,
      const image::Size& inputSize,
      const image::Rect& cropRect);

  ~CroppingScanlineProcessingBlock() override = default;

  void consume(std::unique_ptr<image::Scanline> scanline) override;
  std::unique_ptr<image::Scanline> produce() override;
};

} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
