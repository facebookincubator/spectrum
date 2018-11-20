// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ScanlinePump.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/proc/ScanlineProcessingBlock.h>
#include <spectrum/image/Scanline.h>

#include <functional>
#include <vector>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

void ScanlinePump::pumpAll() {
  for (std::size_t i = 0; i < numInputScanlines; i++) {
    // generate one input scanline
    auto scanline = scanlineGenerator();
    SPECTRUM_ENFORCE_IF_NOT(scanline);

    // exectue processing blocks and consumer while there's actual processing
    // happening in any of the processing steps
    bool change;
    do {
      change = false;
      for (auto& block : processingBlocks) {
        // consume scanline of previous block (possible from input block)
        if (scanline) {
          block->consume(std::move(scanline));
        }

        // set current scanline to output of this block
        SPECTRUM_ENFORCE_IF_NOT(!scanline);
        scanline = block->produce();

        if (scanline) {
          change = true;
        }
      }

      // the scanlineConsumer behaves as a last block that does not produce
      if (scanline) {
        scanlineConsumer(std::move(scanline));
      }

    } while (change);
  }
}

} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
