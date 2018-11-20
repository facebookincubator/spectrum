// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/proc/ScanlineProcessingBlock.h>
#include <spectrum/image/Scanline.h>

#include <functional>
#include <vector>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

/**
 * The scanline pump handles the necessary iterations for "pumping" scanlines
 * from a generator (e.g. a decompressor) through processing blocks (e.g.
 * rotation or scaling) into a final consumer (e.g. a compressor).
 *
 * It allow for processing blocks to buffer any number of scanlines during the
 * process like e.g. the rotation processor does.
 */
class ScanlinePump {
 private:
  std::function<std::unique_ptr<image::Scanline>()> scanlineGenerator;
  std::vector<std::unique_ptr<ScanlineProcessingBlock>> processingBlocks;
  std::function<void(std::unique_ptr<image::Scanline>)> scanlineConsumer;

  const std::size_t numInputScanlines;

 public:
  ScanlinePump(
      std::function<std::unique_ptr<image::Scanline>()> scanlineGenerator,
      std::vector<std::unique_ptr<ScanlineProcessingBlock>> processingBlocks,
      std::function<void(std::unique_ptr<image::Scanline>)> scanlineConsumer,
      const int numInputScanlines)
      : scanlineGenerator(scanlineGenerator),
        processingBlocks(std::move(processingBlocks)),
        scanlineConsumer(scanlineConsumer),
        numInputScanlines(numInputScanlines) {
    SPECTRUM_ENFORCE_IF_NOT(numInputScanlines != 0);
    SPECTRUM_ENFORCE_IF_NOT(scanlineGenerator != nullptr);
    SPECTRUM_ENFORCE_IF_NOT(scanlineConsumer != nullptr);
  }

  void pumpAll();
};


} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
