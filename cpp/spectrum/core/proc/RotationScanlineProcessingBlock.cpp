// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "RotationScanlineProcessingBlock.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/proc/ScanlineProcessingBlock.h>
#include <spectrum/image/Scanline.h>

#include <memory>
#include <vector>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

void RotationScanlineProcessingBlock::consume(
    std::unique_ptr<image::Scanline> scanline) {
  SPECTRUM_ENFORCE_IF_NOT(scanline->specification() == _pixelSpecification);
  SPECTRUM_ENFORCE_IF_NOT(scanline->width() == inputSize.width);
  SPECTRUM_ENFORCE_IF_NOT(input.size() < inputSize.height);
  SPECTRUM_ENFORCE_IF_NOT(outputScanline == 0);
  input.push_back(std::move(scanline));
}

std::unique_ptr<image::Scanline> RotationScanlineProcessingBlock::produce() {
  if ((orientation == image::Orientation::UpMirrored &&
       static_cast<std::size_t>(outputScanline) >= input.size()) ||
      static_cast<std::size_t>(inputSize.height) > input.size()) {
    return nullptr;
  }

  SPECTRUM_ENFORCE_IF_NOT(outputScanline < outputSize.height);

  auto result =
      std::make_unique<image::Scanline>(_pixelSpecification, outputSize.width);

  switch (orientation) {
    case image::Orientation::Up:
      SPECTRUM_ENFORCE_IF_NOT(false);
      break;

    case image::Orientation::UpMirrored:
      for (int idx = 0; idx < outputSize.width; idx++) {
        copyPixelFromTo(
            input[outputScanline], inputSize.width - idx - 1, result, idx);
      }
      break;

    case image::Orientation::Right:
      for (int idx = 0; idx < outputSize.width; idx++) {
        copyPixelFromTo(
            input[inputSize.height - idx - 1], outputScanline, result, idx);
      }
      break;

    case image::Orientation::RightMirrored:
      for (int idx = 0; idx < outputSize.width; idx++) {
        copyPixelFromTo(
            input[inputSize.height - idx - 1],
            inputSize.width - outputScanline - 1,
            result,
            idx);
      }
      break;

    case image::Orientation::Bottom:
      for (int idx = 0; idx < outputSize.width; idx++) {
        copyPixelFromTo(
            input[inputSize.height - outputScanline - 1],
            inputSize.width - idx - 1,
            result,
            idx);
      }
      break;

    case image::Orientation::BottomMirrored:
      for (int idx = 0; idx < outputSize.width; idx++) {
        copyPixelFromTo(
            input[inputSize.height - outputScanline - 1], idx, result, idx);
      }
      break;

    case image::Orientation::Left:
      for (int idx = 0; idx < outputSize.width; idx++) {
        copyPixelFromTo(
            input[idx], inputSize.width - outputScanline - 1, result, idx);
      }
      break;

    case image::Orientation::LeftMirrored:
      for (int idx = 0; idx < outputSize.width; idx++) {
        copyPixelFromTo(input[idx], outputScanline, result, idx);
      }
      break;
  }

  outputScanline++;
  if (outputScanline == outputSize.height) {
    // optimization: if the last output line has been read, it is safe to
    // forget the input scanlines
    input.clear();
  }

  return result;
}

} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
