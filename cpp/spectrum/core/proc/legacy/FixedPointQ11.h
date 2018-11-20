// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

/** ===========================================================================
 *       @file  FixedPointQ11.h
 *      @brief  Q11 conversion utilities
 * ============================================================================
 */

#ifndef __FIXED_POINT_Q11_H_
#define __FIXED_POINT_Q11_H_

// =========================================
// loop unrolling macros for different output components configurations
#define FltToFixQ11(f) ((f) * (1 << 11))

// regular arithmetic macros
#define AddWeightedPixelGrayscale(pacc, psrc, alpha) \
  do {                                               \
    *(pacc) += (*(psrc) * (alpha));                  \
  } while (0)

#define AddWeightedPixelRGB(pacc, psrc, alpha) \
  do {                                         \
    (pacc)[0] += ((psrc)[0] * (alpha));        \
    (pacc)[1] += ((psrc)[1] * (alpha));        \
    (pacc)[2] += ((psrc)[2] * (alpha));        \
  } while (0)

#define AddWeightedPixelRGBA(pacc, psrc, alpha) \
  do {                                          \
    (pacc)[0] += ((psrc)[0] * (alpha));         \
    (pacc)[1] += ((psrc)[1] * (alpha));         \
    (pacc)[2] += ((psrc)[2] * (alpha));         \
    (pacc)[3] += ((psrc)[3] * (alpha));         \
  } while (0)

// Q11 fixed point arithmetic macros
#define AddWeightedPixelFixGrayscale(pacc, psrc, alpha) \
  do {                                                  \
    *(pacc) += ((*(psrc) * (alpha)) >> 11);             \
  } while (0)

#define AddWeightedPixelFixRGB(pacc, psrc, alpha) \
  do {                                            \
    (pacc)[0] += (((psrc)[0] * (alpha)) >> 11);   \
    (pacc)[1] += (((psrc)[1] * (alpha)) >> 11);   \
    (pacc)[2] += (((psrc)[2] * (alpha)) >> 11);   \
  } while (0)

#define AddWeightedPixelFixRGBA(pacc, psrc, alpha)         \
  do {                                                     \
    (pacc)[0] += (((int32_t)((psrc)[0]) * (alpha)) >> 11); \
    (pacc)[1] += (((int32_t)((psrc)[1]) * (alpha)) >> 11); \
    (pacc)[2] += (((int32_t)((psrc)[2]) * (alpha)) >> 11); \
    (pacc)[3] += (((int32_t)((psrc)[3]) * (alpha)) >> 11); \
  } while (0)

// switch between pixel configurations
#define AddWeightedPixel(ocomp, pacc, psrc, alpha)    \
  do {                                                \
    switch (ocomp) {                                  \
      case 1:                                         \
        AddWeightedPixelGrayscale(pacc, psrc, alpha); \
        break;                                        \
      case 3:                                         \
        AddWeightedPixelRGB(pacc, psrc, alpha);       \
        break;                                        \
      case 4:                                         \
        AddWeightedPixelRGBA(pacc, psrc, alpha);      \
        break;                                        \
    }                                                 \
  } while (0)

#define AddWeightedPixelQ11(ocomp, pacc, psrc, alpha)    \
  do {                                                   \
    switch (ocomp) {                                     \
      case 1:                                            \
        AddWeightedPixelFixGrayscale(pacc, psrc, alpha); \
        break;                                           \
      case 3:                                            \
        AddWeightedPixelFixRGB(pacc, psrc, alpha);       \
        break;                                           \
      case 4:                                            \
        AddWeightedPixelFixRGBA(pacc, psrc, alpha);      \
        break;                                           \
    }                                                    \
  } while (0)

#endif // __FIXED_POINT_Q11_H_

// ================================= EOF ======================================
