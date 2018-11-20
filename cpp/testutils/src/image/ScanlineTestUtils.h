// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Scanline.h>
#include <spectrum/testutils/image/PixelTestUtils.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace testutils {

//
// makeScanline
//

std::unique_ptr<image::Scanline> makeScanline(
    const image::pixel::Specification& pixelSpecification,
    std::vector<std::vector<std::uint8_t>>&& pixels);

std::unique_ptr<image::Scanline> makeScanlineGray(
    pixel::testutils::InitializerVector&& pixels);

std::unique_ptr<image::Scanline> makeScanlineRgb(
    pixel::testutils::InitializerVector&& pixels);

std::unique_ptr<image::Scanline> makeScanlineRgba(
    pixel::testutils::InitializerVector&& pixels);

std::unique_ptr<image::Scanline> makeScanlineArgb(
    pixel::testutils::InitializerVector&& pixels);

//
// assertScanline
//

::testing::AssertionResult assertScanline(
    std::vector<std::vector<std::uint8_t>>&& pixels,
    const image::pixel::Specification& expectedPixelSpecification,
    const image::Scanline* const scanline);

::testing::AssertionResult assertScanlineGray(
    pixel::testutils::InitializerVector&& pixels,
    const image::Scanline* const scanline);

::testing::AssertionResult assertScanlineRgb(
    pixel::testutils::InitializerVector&& pixels,
    const image::Scanline* const scanline);

::testing::AssertionResult assertScanlineArgb(
    pixel::testutils::InitializerVector&& pixels,
    const image::Scanline* const scanline);

::testing::AssertionResult assertScanlineRgba(
    pixel::testutils::InitializerVector&& pixels,
    const image::Scanline* const scanline);

//
// assertColor
//

void assertScanlineIsColor(
    const image::Scanline& scanline,
    const image::pixel::Specification& expectedPixelSpecification,
    pixel::testutils::Initializer&& expectedPixel);

void assertScanlineIsColorGray(
    const image::Scanline& scanline,
    pixel::testutils::Initializer&& expectedPixel);

void assertScanlineIsColorRgb(
    const image::Scanline& scanline,
    pixel::testutils::Initializer&& expectedPixel);

void assertScanlineIsColorRgba(
    const image::Scanline& scanline,
    pixel::testutils::Initializer&& expectedPixel);

void assertScanlineIsColorArgb(
    const image::Scanline& scanline,
    pixel::testutils::Initializer&& expectedPixel);

} // namespace testutils
} // namespace image
} // namespace spectrum
} // namespace facebook
