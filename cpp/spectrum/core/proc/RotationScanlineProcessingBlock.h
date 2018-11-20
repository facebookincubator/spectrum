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
#include <vector>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

/**
 * Reads scanlines from an input image and rotates it by either 90, 180 or 270
 * degree. Output scanlines are only produced once the entire image has been
 * consumed.
 */
class RotationScanlineProcessingBlock : public ScanlineProcessingBlock {
 private:
  const image::pixel::Specification _pixelSpecification;
  const image::Size inputSize;
  const image::Size outputSize;
  const image::Orientation orientation;

  std::vector<std::unique_ptr<image::Scanline>> input = {};
  int outputScanline = 0;

 public:
  RotationScanlineProcessingBlock(
      const image::pixel::Specification& pixelSpecification,
      const image::Size inputSize,
      const image::Orientation orientation)
      : _pixelSpecification(pixelSpecification),
        inputSize(inputSize),
        outputSize(inputSize.oriented(orientation)),
        orientation(orientation) {
    SPECTRUM_ENFORCE_IF_NOT(orientation != image::orientationDefault);
  }
  ~RotationScanlineProcessingBlock() override{};

  void consume(std::unique_ptr<image::Scanline> scanline) override;
  std::unique_ptr<image::Scanline> produce() override;
};

} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
