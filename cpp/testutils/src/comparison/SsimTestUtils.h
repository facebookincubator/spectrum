// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/testutils/TestUtils.h>

#include <random>

namespace facebook {
namespace spectrum {
namespace comparison {
namespace testutils {

using Rng = std::mt19937;

std::unique_ptr<image::Scanline> sampleScanline(
    Rng& rng,
    const image::pixel::Specification& spec,
    const std::size_t width);

std::vector<std::unique_ptr<image::Scanline>> sampleImage(
    Rng& rng,
    const image::pixel::Specification& spec,
    const image::Size size);

std::vector<std::unique_ptr<image::Scanline>> makeImageFill(
    const image::pixel::Specification& spec,
    const image::Size size,
    const std::vector<std::uint8_t>&& pixel);

std::vector<std::unique_ptr<image::Scanline>> readJpegImage(
    const std::string& path);

} // namespace testutils
} // namespace comparison
} // namespace spectrum
} // namespace facebook
