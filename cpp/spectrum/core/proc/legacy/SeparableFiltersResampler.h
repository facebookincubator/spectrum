// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

/** ===========================================================================
 *       @file  SeparableFiltersResampler.h
 *      @brief  downsampling using magic kernel, separable and online
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
// filters a 2D image using a separable kernel, operating in an online,
// row by row manner
class SeparableFiltersResampler {
 public:
  SeparableFiltersResampler(
      const std::uint32_t& w_src,
      const std::uint32_t& h_src,
      const std::uint32_t& w_dst,
      const std::uint32_t& h_dst,
      const std::uint32_t& ocomp);

  virtual ~SeparableFiltersResampler() {}

  void putLine(const std::uint8_t* pSrc);
  int32_t* getLine();

 private:
  // consts
  static const std::size_t NUM_SHARPEN_BUFFERS;
  static const float MK_SUPPORT;

  typedef struct Contrib {
    int index;
    float weight;
    int32_t wQ;
    Contrib(int i, float w) : index(i), weight(w), wQ(0) {}
  } Contrib;

  // metadata
  std::uint32_t mSrcWidth;
  std::uint32_t mSrcHeight;
  std::uint32_t mDstWidth;
  std::uint32_t mDstHeight;
  std::uint8_t mOutputComponents;
  std::uint32_t mDstPitch;

  // counters
  int mSrcY;
  int mDstY;
  std::size_t mIntermediateHead;
  std::size_t mIntermediateTail;

  // lists of contributors and corresponding weights
  std::vector<std::vector<Contrib>> mXContributors;
  std::vector<std::vector<Contrib>> mYContributors;

  // map: [src row idx] -> <index in intermediate buffers, #times accessed>
  std::vector<std::pair<std::size_t, int>> mSrcRowInfo;

  // intermediate storage
  std::vector<std::vector<int32_t>> mIntermediateRows;
  std::vector<int32_t> mYResamplingBuffer;

  // output buffer
  std::vector<std::uint8_t> mOutBuffer;

  // internal methods
  std::size_t prepareContributorLists();
  float magicKernelWeight(float z);
  void resampleX(const std::uint8_t* pSrc, int32_t* pDst);
  void resampleY();
  void start();
};

} // namespace legacy
} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook

// ================================= EOF ======================================
