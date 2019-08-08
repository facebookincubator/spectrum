// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <folly/Range.h>

#include <cstddef>
#include <string>

namespace facebook {
namespace spectrum {
namespace testdata {

struct Path {
  folly::StringPiece path;

  std::string normalized() const;

  constexpr Path(folly::StringPiece&& path) : path(path) {}
};

namespace paths {

static constexpr Path NON_EXISTING_FILE =
    folly::StringPiece{"some/path/that/does/not/exists.jpg"};

namespace jpeg {

static constexpr Path s128x85_Q75_BASELINE = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/128x85_q75_baseline.jpg"};

static constexpr Path s128x85_Q75_GRAYSCALE = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/128x85_q75_grayscale.jpg"};

static constexpr Path s128x85_Q75_PROGRESSIVE = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/128x85_q75_progressive.jpg"};

static constexpr Path s800x530_Q75_BASELINE = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/800x530_q75_baseline.jpg"};

static constexpr Path s800x530_Q75_PROGRESSIVE = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/800x530_q75_progressive.jpg"};

static constexpr Path s16x16_WHITE_Q75 = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/16x16_white_q75.jpg"};

static constexpr Path s16x16_BLACK_Q75 = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/16x16_black_q75.jpg"};

static constexpr Path s16x16_WHITE_Q75_GRAYSCALE = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/16x16_white_q75_grayscale.jpg"};

static constexpr Path s16x16_BLACK_Q75_GRAYSCALE = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/16x16_black_q75_grayscale.jpg"};

static constexpr Path s16x16_cABCDF0_Q75 = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/16x16_cABCDF0_q75.jpg"};

static constexpr Path s16x16_cABCDF0_Q75_sampling444 = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/jpeg/16x16_cABCDF0_q75_sampling444.jpg"};

static constexpr Path s128x85_Q75_BASELINE_MALFORMED_SAMSUNG_STYLE =
    folly::StringPiece{
        "xplat/spectrum/cpp/testutils/data/jpeg/128x85_q75_baseline_malformed_samsung_style.jpg"};

static constexpr Path s85x128_Q15 =
    folly::StringPiece{"xplat/spectrum/cpp/testutils/data/jpeg/85x128_q15.jpg"};

static constexpr Path s85x128_Q85 =
    folly::StringPiece{"xplat/spectrum/cpp/testutils/data/jpeg/85x128_q85.jpg"};

} // namespace jpeg

namespace png {

static constexpr Path s128x85_RGB =
    folly::StringPiece{"xplat/spectrum/cpp/testutils/data/png/128x85_rgb.png"};
static constexpr Path s128x85_RGB_INTERLACED = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/128x85_rgb_interlaced.png"};
static constexpr Path s128x85_RGB_TRUNCATED = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/128x85_rgb_truncated.png"};

static constexpr Path s16x16_BLACK_GRAY_1B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_black_gray_1b.png"};
static constexpr Path s16x16_BLACK_GRAY_2B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_black_gray_2b.png"};
static constexpr Path s16x16_BLACK_GRAY_4B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_black_gray_4b.png"};
static constexpr Path s16x16_BLACK_GRAY_8B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_black_gray_8b.png"};

static constexpr Path s16x16_BLACK_GRAY_A_8B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_black_gray_a_8b.png"};
static constexpr Path s16x16_BLACK_GRAY_A_16B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_black_gray_a_16b.png"};
static constexpr Path s16x16_BLACK_GRAY_TRNS_8B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_black_gray_tRNS_8b.png"};
static constexpr Path s16x16_BLACK_GRAY_TRNS_16B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_black_gray_tRNS_16b.png"};

static constexpr Path s16x16_WHITE_GRAY_1B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_white_gray_1b.png"};
static constexpr Path s16x16_WHITE_GRAY_2B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_white_gray_2b.png"};
static constexpr Path s16x16_WHITE_GRAY_4B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_white_gray_4b.png"};
static constexpr Path s16x16_WHITE_GRAY_8B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_white_gray_8b.png"};

static constexpr Path s16x16_cABCDEF_RGB_4B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_cABCDEF_rgb_4b.png"};
static constexpr Path s16x16_cABCDEF_RGB_8B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_cABCDEF_rgb_8b.png"};
static constexpr Path s16x16_cABCDEF_RGB_8B_INTERLACED = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_cABCDEF_rgb_8b_interlaced.png"};
static constexpr Path s16x16_cABCDEF_RGB_16B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_cABCDEF_rgb_16b.png"};

static constexpr Path s16x16_a50_cABCDEF_ARGB_8B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/png/16x16_a50_cABCDEF_argb_8b.png"};
} // namespace png

namespace webp {
static constexpr Path s16x16_WHITE_GRAY_1B = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/16x16_white_gray_1b.webp"};
static constexpr Path s16x16_WHITE_GRAY_1B_LOSSLESS = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/16x16_white_gray_1b_lossless.webp"};
static constexpr Path s32x32_ANIMATION = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/32x32_animation.webp"};

static constexpr Path s128x85_RGB_LOSSLESS = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/128x85_rgb_lossless.webp"};
static constexpr Path s128x85_RGB_LOSSY = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/128x85_rgb_lossy.webp"};
static constexpr Path s400x301_WITH_ALPHA_LOSSLESS = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/400x301_lossless_with_alpha.webp"};
static constexpr Path s400x301_WITH_ALPHA_LOSSY = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/400x301_lossless_with_alpha.webp"};

static constexpr Path s16x16_cABCDEF_RGB_LOSSLESS = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/16x16_cABCDEF_lossless.webp"};
static constexpr Path s16x16_a50_cABCDEF_ARGB_LOSSLESS = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/16x16_a_50_cABCDEF_lossless.webp"};

static constexpr Path exceedingly_large = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/webp/exceedingly_large.webp"};

} // namespace webp

namespace avif {

static constexpr Path s256_170_rav1e_s420 = folly::StringPiece{
    "xplat/spectrum/cpp/testutils/data/avif/256_170_rav1e_s420.ivf"};

}

namespace misc {
static constexpr Path alphabetTxt =
    folly::StringPiece{"xplat/spectrum/cpp/testutils/data/misc/alphabet.txt"};
}

} // namespace paths
} // namespace testdata
} // namespace spectrum
} // namespace facebook
