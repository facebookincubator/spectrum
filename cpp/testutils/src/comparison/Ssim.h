// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <memory>
#include <vector>

#include <spectrum/image/Scanline.h>

namespace facebook {
namespace spectrum {
namespace comparison {
namespace testutils {

namespace error {
extern const folly::StringPiece ImageTooSmall;
extern const folly::StringPiece ImageSizesDontMatch;
extern const folly::StringPiece ImageWrongSpecification;
} // namespace error

/**
 * Compute the structural similarity (SSIM) between two images using the
 * algorithm described in: http://www.cns.nyu.edu/pub/eero/wang03-reprint.pdf.
 *
 * @return The mean ssim of all channels between the two images.
 * @note Both images should have the same size and pixel specification. Both
 * images should also be larger than 8x8.
 */
float compareSsim(
    const image::pixel::Specification& spec,
    const std::vector<std::unique_ptr<image::Scanline>>& imgA,
    const std::vector<std::unique_ptr<image::Scanline>>& imgB);

} // namespace testutils
} // namespace comparison
} // namespace spectrum
} // namespace facebook
