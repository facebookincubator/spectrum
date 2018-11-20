// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Scanline.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {

/**
 * A ScanlineProcessingBlock is a buffer that consumes scanlines, applies
 * modifications to one or all read scanlines and then generates new
 * scanlines.
 *
 * When a ScanlineProcessingBlock consumes a scanline it takes over ownership
 * and with the destruction of the ScanlineProcessingBlock, the scanline is also
 * freed.
 *
 * There is no required 1:1 relation between consumed and produced scanline. For
 * instance, a scaling processing block might produce less scanlines than it
 * consumes.
 */
class ScanlineProcessingBlock {
 public:
  virtual ~ScanlineProcessingBlock() = default;

  /**
   * Consume a scanline as input and take ownership
   */
  virtual void consume(std::unique_ptr<image::Scanline> scanline) = 0;

  /**
   * Produce a scanline as output. When there's no scanline ready, a nullptr
   * is returned. If a nullptr is returned after the last scanline has been
   * fed to "consume", the block is finished and will not produce further
   * output.
   */
  virtual std::unique_ptr<image::Scanline> produce() = 0;
};

} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
