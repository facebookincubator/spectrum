// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

/** ===========================================================================
 *       @file  SeparableFiltersResampler.cpp
 *      @brief  downsampling using magic kernel, separable and online
 * ============================================================================
 */

// ============= include files =============
#include "SeparableFiltersResampler.h"
#include "FixedPointQ11.h"

#include <spectrum/core/SpectrumEnforce.h>

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
// static variables
const std::size_t SeparableFiltersResampler::NUM_SHARPEN_BUFFERS = 3;
const float SeparableFiltersResampler::MK_SUPPORT = 3.0f;

// =========================================
// init the mks resizer
SeparableFiltersResampler::SeparableFiltersResampler(
    const std::uint32_t& w_src,
    const std::uint32_t& h_src,
    const std::uint32_t& w_dst,
    const std::uint32_t& h_dst,
    const std::uint32_t& ocomp)
    : mSrcWidth(w_src),
      mSrcHeight(h_src),
      mDstWidth(w_dst),
      mDstHeight(h_dst),
      mOutputComponents(ocomp),
      mSrcY(0),
      mDstY(0),
      mIntermediateHead(0),
      mIntermediateTail(0) {
  start();
}

// =========================================
// prepare internal structs
void SeparableFiltersResampler::start() {
  mXContributors.clear();
  mYContributors.clear();
  mSrcRowInfo.clear();
  mIntermediateRows.clear();
  mYResamplingBuffer.clear();
  mOutBuffer.clear();

  // prepare filtering data
  mXContributors.resize(mDstWidth);
  mYContributors.resize(mDstHeight);
  mSrcRowInfo.resize(mSrcHeight);
  std::fill(mSrcRowInfo.begin(), mSrcRowInfo.end(), make_pair(0, 0));
  std::size_t nRowsBuffered = prepareContributorLists();

  // allocate internal buffers
  mIntermediateRows.resize(nRowsBuffered);
  mDstPitch = mDstWidth * mOutputComponents;
  for (std::size_t j = 0; j < nRowsBuffered; ++j) {
    mIntermediateRows[j].resize(mDstPitch);
  }
  mYResamplingBuffer.resize(mDstPitch);
  mOutBuffer.resize(mDstPitch);
}

// =========================================
// handle input scanline, std::uint8_t values
void SeparableFiltersResampler::putLine(const std::uint8_t* pSrc) {
  // sanity: if this src row does not contribute to any dst pixel, do nothing
  if (!mSrcRowInfo[mSrcY].second) {
    ++mSrcY;
    return;
  }

  // get available row to write into
  mSrcRowInfo[mSrcY].first = mIntermediateTail;
  int32_t* pOutputRow = mIntermediateRows[mIntermediateTail].data();
  resampleX(pSrc, pOutputRow);

  // manage intermediate buffers ring buffer
  mIntermediateTail = (mIntermediateTail + 1) % mIntermediateRows.size();
  if (mIntermediateTail == mIntermediateHead) {
    mIntermediateHead = (mIntermediateHead + 1) % mIntermediateRows.size();
  }
}

// =========================================
// pointer to output row, in Q21.11
int32_t* SeparableFiltersResampler::getLine() {
  if (mDstY >= (int)mDstHeight) {
    return NULL;
  }
  if (mSrcY != mYContributors[mDstY].back().index) {
    ++mSrcY;
    return NULL;
  }
  resampleY();
  return mYResamplingBuffer.data();
}

// =========================================
// resample in x dimension
void SeparableFiltersResampler::resampleX(
    const std::uint8_t* pSrc,
    int32_t* pDst) {
  memset(pDst, 0, mDstPitch * sizeof(int32_t));
  auto end = mXContributors.cend();
  for (auto clists = mXContributors.cbegin(); clists != end; ++clists) {
    auto clists_end = clists->cend();
    for (auto contributors = clists->cbegin(); contributors != clists_end;
         ++contributors) {
      const int32_t w = contributors->wQ;
      const std::uint8_t* pPixel =
          pSrc + (contributors->index * mOutputComponents);
      // multiply and convert to Q21.11
      AddWeightedPixel(mOutputComponents, pDst, pPixel, w);
    }
    pDst += mOutputComponents;
  }
}

// =========================================
// resample in y dimension
void SeparableFiltersResampler::resampleY() {
  std::fill(mYResamplingBuffer.begin(), mYResamplingBuffer.end(), 0);
  int32_t* pDst = mYResamplingBuffer.data();

  // for each x in w_dst
  for (std::uint32_t x = 0; x < mDstWidth; ++x) {
    auto end = mYContributors[mDstY].cend();
    for (auto contributors = mYContributors[mDstY].begin(); contributors != end;
         ++contributors) {
      const int32_t w = contributors->wQ;
      const int32_t* pPixel =
          mIntermediateRows[mSrcRowInfo[contributors->index].first].data() +
          (x * mOutputComponents);
      // pPixel is in Q21.11
      AddWeightedPixelQ11(mOutputComponents, pDst, pPixel, w);
    }
    pDst += mOutputComponents;
  }
  ++mDstY;
}

// =========================================
// init contributors list
std::size_t SeparableFiltersResampler::prepareContributorLists() {
  // count number of rows that need to be buffered to allow y resizing
  std::size_t nRowsBuffered = 0;

  // for each direction
  for (int d = 1; d >= 0; --d) {
    const std::uint32_t& dst = (d ? mDstWidth : mDstHeight);
    const std::uint32_t& src = (d ? mSrcWidth : mSrcHeight);
    const double scale = static_cast<double>(dst) / src;
    const double filterHalfWidth = ((MK_SUPPORT * 0.5f) / scale);

    // for each dst coordinate, identify the src coordinates which contribute
    // to it
    for (std::uint32_t c = 0; c < dst; ++c) {
      vector<Contrib>& contrib = (d ? mXContributors[c] : mYContributors[c]);

      // convert from discrete to continuous coordinates, scale, then
      // convert back to discrete.
      float center = ((static_cast<float>(c) + 0.5f) / scale) - 0.5f;
      int left = static_cast<int>(floor(center - filterHalfWidth));
      int right = static_cast<int>(ceil(center + filterHalfWidth));

      // calculate index and weight for each contributor
      float totalWeight = 0.0f;
      float maxWeight = -INFINITY;
      std::size_t maxWeightIndex = 0;
      for (int j = left; j <= right; ++j) {
        // get the weight associated with this index
        float weight = magicKernelWeight((center - j) * scale);
        if (weight == 0.0f) {
          continue;
        }
        totalWeight += weight;

        // ensure index is valid (clamp if necessary)
        int index = j;
        if (index < 0) {
          index = 0;
        } else if (index >= (int)src) {
          index = src - 1;
        }

        // record this contributor
        contrib.push_back(Contrib(index, weight));
        if (weight > maxWeight) {
          maxWeight = weight;
          maxWeightIndex = contrib.size() - 1;
        }
      }

      // normalize
      const double norm = (1.0f / totalWeight);
      totalWeight = 0.0f;
      for (std::size_t j = 0; j < contrib.size(); ++j) {
        contrib[j].weight *= norm;
        contrib[j].wQ = FltToFixQ11(contrib[j].weight);
        totalWeight += contrib[j].weight;
      }

      // ensure filtered values add up to 1
      if (totalWeight != 1.0f) {
        contrib[maxWeightIndex].weight += (1.0f - totalWeight);
        contrib[maxWeightIndex].wQ =
            FltToFixQ11(contrib[maxWeightIndex].weight);
      }

      // buffered rows logic
      if (d == 0) {
        nRowsBuffered = std::max(nRowsBuffered, contrib.size());
      }
    }
  }

  // for each src row, count how many times it contributes to dst
  for (std::uint32_t r = 0; r < mDstHeight; ++r) {
    for (std::size_t k = 0; k < mYContributors[r].size(); ++k) {
      SPECTRUM_ENFORCE_IF(mYContributors[r][k].index < 0);
      SPECTRUM_ENFORCE_IF(mYContributors[r][k].index >= (int)mSrcHeight);
      mSrcRowInfo[mYContributors[r][k].index].second++;
    }
  }
  return nRowsBuffered;
}

// =========================================
// return the associated magic kernel weight of index z
float SeparableFiltersResampler::magicKernelWeight(float z) {
  if (z < -1.5 || z > 1.5) {
    return 0;
  }
  if (z < -0.5) {
    double y = z + 1.5;
    return (0.5 * y * y);
  }
  if (z < 0.5) {
    return (0.75 - (z * z));
  }
  double y = (z - 1.5);
  return (0.5 * y * y);
}

} // namespace legacy
} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook

// ================================= EOF ======================================
