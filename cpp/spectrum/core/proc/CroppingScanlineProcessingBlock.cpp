// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "CroppingScanlineProcessingBlock.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/proc/ScanlineProcessingBlock.h>
#include <spectrum/image/Scanline.h>

#include <memory>
#include <queue>
#include <stdexcept>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

CroppingScanlineProcessingBlock::CroppingScanlineProcessingBlock(
    const image::pixel::Specification& pixelSpecification,
    const image::Size& inputSize,
    const image::Rect& cropRect)
    : _pixelSpecification(pixelSpecification),
      inputSize(inputSize),
      cropRect(cropRect) {
  SPECTRUM_ENFORCE_IF(inputSize.empty());
  SPECTRUM_ENFORCE_IF(cropRect.size.empty());
  SPECTRUM_ENFORCE_IF_NOT(inputSize.contains(cropRect.bottomRight()));
}

void CroppingScanlineProcessingBlock::consume(
    std::unique_ptr<image::Scanline> scanline) {
  SPECTRUM_ENFORCE_IF_NOT(scanline->specification() == _pixelSpecification);
  SPECTRUM_ENFORCE_IF_NOT(scanline->width() == inputSize.width);
  SPECTRUM_ENFORCE_IF_NOT(inputScanline < inputSize.height);
  SPECTRUM_ENFORCE_IF_NOT(output.size() <= cropRect.size.height);

  if (inputScanline < cropRect.minY() || inputScanline >= cropRect.maxY()) {
    // drop scanlines before and after output area
    inputScanline++;
    return;
  }

  inputScanline++;

  if (scanline->width() == cropRect.size.width) {
    output.push(std::move(scanline));

  } else {
    auto outputScanline = std::make_unique<image::Scanline>(
        _pixelSpecification, cropRect.size.width);

    // TODO t21061498: Optimize using SIMD instructions (or memcpy)
    for (std::size_t i = 0; i < cropRect.size.width; i++) {
      copyPixelFromTo(scanline, cropRect.topLeft.x + i, outputScanline, i);
    }

    output.push(std::move(outputScanline));
  }
}

std::unique_ptr<image::Scanline> CroppingScanlineProcessingBlock::produce() {
  SPECTRUM_ENFORCE_IF_NOT(output.size() <= cropRect.size.height);

  if (output.empty()) {
    return nullptr;
  } else {
    auto result = std::move(output.front());
    SPECTRUM_ENFORCE_IF_NOT(result);
    output.pop();
    return result;
  }
}

} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
