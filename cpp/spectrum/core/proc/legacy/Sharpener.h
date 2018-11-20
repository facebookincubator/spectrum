// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

/** ===========================================================================
 *       @file  Sharpener.h
 *      @brief  super sampling resizer (scanline version)
 * ============================================================================
 */

#pragma once

// ============= include files =============
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <vector>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {
namespace legacy {

// =========================================
// applies a 3x3 sharpening filter on a 2D image, online (row by row) operation
class Sharpener {
 public:
  Sharpener(
      const std::uint32_t& w_src,
      const std::uint32_t& h_src,
      const std::uint32_t& ocomp,
      std::uint8_t* pOutBuffer);

  virtual ~Sharpener() {}

  void putLine(const int32_t* pSrc);
  std::uint8_t* getLine(std::uint8_t* pOutBuffer = NULL);

 private:
  // consts
  static const std::size_t KERNEL_WIDTH = 3;
  static const int32_t KERNEL_Q11[KERNEL_WIDTH];

  // metadata
  std::uint32_t mWidth;
  std::uint32_t mHeight;
  std::uint8_t mOutputComponents;
  std::uint32_t mPitch;

  // counters
  std::uint32_t mRow;
  std::size_t mHead;
  std::size_t mTail;
  bool mValid;

  // intermediate storage
  std::vector<std::vector<int32_t>> mIntermediateRows;
  std::vector<int32_t> mYSharpenBuffer;

  // output buffer
  std::vector<std::uint8_t> mOutBuffer;
  std::uint8_t* mOutBufferPtr;

  void sharpenX(const int32_t* pSrc);
  void sharpenY();
  void updateRingBuffer();
};

} // namespace legacy
} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook

// ================================= EOF ======================================
