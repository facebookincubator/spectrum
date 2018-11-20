// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

/** ===========================================================================
 *       @file  Sharpener.cpp
 *      @brief  2D Image Sharpener (3x3 kernel, seperable)
 * ============================================================================
 */

// ============= include files =============
#include "Sharpener.h"
#include "FixedPointQ11.h"

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>

using namespace std;

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {
namespace legacy {

// =========================================
// sharpen kernel
const int32_t Sharpener::KERNEL_Q11[3] = {
    -506, // FltToFixQ11(-0.2473)
    3060, // FltToFixQ11(1.4946)
    -506 // FltToFixQ11(-0.2473)
};

// =========================================
// constructor
Sharpener::Sharpener(
    const std::uint32_t& w_src,
    const std::uint32_t& h_src,
    const std::uint32_t& ocomp,
    std::uint8_t* pOutBuffer)
    : mWidth(w_src),
      mHeight(h_src),
      mOutputComponents(ocomp),
      mRow(0),
      mHead(0),
      mTail(0),
      mValid(false),
      mOutBufferPtr(pOutBuffer) {
  mPitch = mWidth * mOutputComponents;
  mIntermediateRows.resize(KERNEL_WIDTH);
  for (std::size_t r = 0; r < KERNEL_WIDTH; ++r) {
    mIntermediateRows[r].resize(mPitch);
  }
  mYSharpenBuffer.resize(mPitch);
  if (!mOutBufferPtr) {
    mOutBuffer.resize(mPitch);
    mOutBufferPtr = mOutBuffer.data();
  }
}

// =========================================
// return a pointer to the output line. also handle last row drain
std::uint8_t* Sharpener::getLine(std::uint8_t* pOutBuffer) {
  if (pOutBuffer) {
    mOutBufferPtr = pOutBuffer;
  }
  if (!mValid) {
    return NULL;
  }
  if (mRow == mHeight) {
    ++mRow;
    mValid = true;
  } else {
    if (mRow > mHeight) {
      putLine(NULL);
    }
    mValid = false;
  }
  return mOutBufferPtr;
}

// =========================================
// input a line to the sharpener
void Sharpener::putLine(const int32_t* pSrc) {
  mValid = false;
  if (pSrc) {
    sharpenX(pSrc);
  }

  // if not enough rows were input, return
  ++mRow;
  if (mRow == 1) {
    updateRingBuffer();
    return;
  }
  sharpenY();

  // convert from Q11 to std::uint8_t
  std::uint8_t* pOut = mOutBufferPtr;
  const int32_t* pBuf = mYSharpenBuffer.data();
  for (std::uint32_t x = 0; x < mPitch; ++x) {
    int32_t val = (((int64_t)(*pBuf++) * (1 << 21)) + ((int64_t)1 << 31)) >> 32;
    *pOut++ = (val < 0 ? 0 : (val > 255 ? 255 : val));
  }

  if (mRow < mHeight) {
    updateRingBuffer();
  }
  mValid = true;
}

// =========================================
// sharpen along x dimension
void Sharpener::sharpenX(const int32_t* pSrc) {
  if (!pSrc) {
    return;
  }
  int32_t* pBuf = mIntermediateRows[mTail].data();
  const std::uint8_t ocomp = mOutputComponents;
  memset(pBuf, 0, mPitch * sizeof(int32_t));
  // first pixel
  AddWeightedPixelQ11(ocomp, pBuf, pSrc, KERNEL_Q11[0]);
  AddWeightedPixelQ11(ocomp, pBuf, pSrc, KERNEL_Q11[1]);
  AddWeightedPixelQ11(ocomp, pBuf, pSrc + ocomp, KERNEL_Q11[2]);
  pBuf += ocomp;
  // mid section
  for (std::uint32_t x = 1; x < mWidth - 1; x++) {
    AddWeightedPixelQ11(ocomp, pBuf, pSrc + ((x - 1) * ocomp), KERNEL_Q11[0]);
    AddWeightedPixelQ11(ocomp, pBuf, pSrc + ((x)*ocomp), KERNEL_Q11[1]);
    AddWeightedPixelQ11(ocomp, pBuf, pSrc + ((x + 1) * ocomp), KERNEL_Q11[2]);
    pBuf += ocomp;
  }
  // last pixel
  AddWeightedPixelQ11(
      ocomp, pBuf, pSrc + ((mWidth - 2) * ocomp), KERNEL_Q11[0]);
  AddWeightedPixelQ11(
      ocomp, pBuf, pSrc + ((mWidth - 1) * ocomp), KERNEL_Q11[1]);
  AddWeightedPixelQ11(
      ocomp, pBuf, pSrc + ((mWidth - 1) * ocomp), KERNEL_Q11[2]);
}

// =========================================
// sharpen along y dimension
void Sharpener::sharpenY() {
  // assign rows, handle edge cases
  int32_t* pRow0 = NULL;
  int32_t* pRow1 = NULL;
  int32_t* pRow2 = NULL;
  if (mRow == 2) {
    // first row
    pRow0 = (mIntermediateRows[0].data());
    pRow1 = (mIntermediateRows[0].data());
    pRow2 = (mIntermediateRows[1].data());
  } else if (mRow > mHeight) {
    // last row
    pRow0 = (mIntermediateRows[(mHead + 1) % KERNEL_WIDTH].data());
    pRow1 = (mIntermediateRows[mTail].data());
    pRow2 = (mIntermediateRows[mTail].data());
  } else {
    pRow0 = (mIntermediateRows[mHead].data());
    pRow1 = (mIntermediateRows[(mHead + 1) % KERNEL_WIDTH].data());
    pRow2 = (mIntermediateRows[mTail].data());
  }

  // sharpen across y dimension
  std::fill(mYSharpenBuffer.begin(), mYSharpenBuffer.end(), 0);
  int32_t* pBuf = mYSharpenBuffer.data();

  for (std::uint32_t x = 0; x < mWidth; ++x) {
    AddWeightedPixelQ11(mOutputComponents, pBuf, pRow0, KERNEL_Q11[0]);
    AddWeightedPixelQ11(mOutputComponents, pBuf, pRow1, KERNEL_Q11[1]);
    AddWeightedPixelQ11(mOutputComponents, pBuf, pRow2, KERNEL_Q11[2]);
    pBuf += mOutputComponents;
    pRow0 += mOutputComponents;
    pRow1 += mOutputComponents;
    pRow2 += mOutputComponents;
  }
}

// =========================================
// manage intermediate buffers ring buffer
void Sharpener::updateRingBuffer() {
  mTail = (mTail + 1) % KERNEL_WIDTH;
  if (mTail == mHead) {
    mHead = (mHead + 1) % KERNEL_WIDTH;
  }
}

} // namespace legacy
} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook

// ================================= EOF ======================================
