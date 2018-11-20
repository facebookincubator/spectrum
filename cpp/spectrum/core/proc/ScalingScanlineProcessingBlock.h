// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/proc/ScanlineProcessingBlock.h>
#include <spectrum/core/proc/legacy/SeparableFiltersResampler.h>
#include <spectrum/core/proc/legacy/Sharpener.h>
#include <spectrum/image/Geometry.h>
#include <spectrum/image/Scanline.h>

#include <cmath>
#include <memory>
#include <vector>

#include <folly/Range.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

class ScalingBlockImpl;

/**
 * Processing block that is able to down-scale and up-scale an input image. The
 * consumed scanlines are freed as soon as possible.
 */
class ScalingScanlineProcessingBlock : public ScanlineProcessingBlock {
 private:
  const image::pixel::Specification _pixelSpecification;
  std::unique_ptr<ScalingBlockImpl> delegate;

 public:
  ScalingScanlineProcessingBlock(
      const image::pixel::Specification& pixelSpecification,
      const image::Size& inputSize,
      const image::Size& outputSize,
      const Configuration::General::SamplingMethod samplingMethod);
  ~ScalingScanlineProcessingBlock() override;

  void consume(std::unique_ptr<image::Scanline> scanline) override;
  std::unique_ptr<image::Scanline> produce() override;
};

} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
