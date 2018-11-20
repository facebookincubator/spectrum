// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ScanlineTestUtils.h"

namespace facebook {
namespace spectrum {
namespace image {
namespace testutils {

//
// makeScanline
//

std::unique_ptr<image::Scanline> makeScanline(
    const image::pixel::Specification& pixelSpecification,
    std::vector<std::vector<std::uint8_t>>&& pixels) {
  auto result =
      std::make_unique<image::Scanline>(pixelSpecification, pixels.size());

  for (std::size_t i = 0; i < pixels.size(); ++i) {
    // Ensuring each input pixel has the right amount of components.
    EXPECT_EQ(pixelSpecification.bytesPerPixel, pixels[i].size());

    std::memcpy(
        result->dataAtPixel(i),
        pixels[i].data(),
        pixelSpecification.bytesPerPixel);
  }

  return result;
}

std::unique_ptr<image::Scanline> makeScanlineGray(
    pixel::testutils::InitializerVector&& pixels) {
  return makeScanline(image::pixel::specifications::Gray, std::move(pixels));
}

std::unique_ptr<image::Scanline> makeScanlineRgb(
    pixel::testutils::InitializerVector&& pixels) {
  return makeScanline(image::pixel::specifications::RGB, std::move(pixels));
}

std::unique_ptr<image::Scanline> makeScanlineRgba(
    pixel::testutils::InitializerVector&& pixels) {
  return makeScanline(image::pixel::specifications::RGBA, std::move(pixels));
}

std::unique_ptr<image::Scanline> makeScanlineArgb(
    pixel::testutils::InitializerVector&& pixels) {
  return makeScanline(image::pixel::specifications::ARGB, std::move(pixels));
}

//
// assertScanline
//

::testing::AssertionResult assertScanline(
    std::vector<std::vector<std::uint8_t>>&& pixels,
    const image::pixel::Specification& expectedPixelSpecification,
    const image::Scanline* const scanline) {
  if (scanline == nullptr) {
    return ::testing::AssertionFailure() << "scanline is null";
  }

  if (pixels.size() != scanline->width()) {
    return ::testing::AssertionFailure() << "list.size() != scanline->width()";
  }

  if (scanline->specification() != expectedPixelSpecification) {
    return ::testing::AssertionFailure()
        << "expectedPixelSpecification != scanline->specification()";
  }

  for (std::size_t i = 0; i < pixels.size(); ++i) {
    if (pixels[i].size() != expectedPixelSpecification.bytesPerPixel) {
      return ::testing::AssertionFailure()
          << "unexpected number of components in pixel";
    }

    const auto pixelOffsetInScanline =
        i * expectedPixelSpecification.bytesPerPixel;

    if (pixelOffsetInScanline + expectedPixelSpecification.bytesPerPixel >
            scanline->sizeBytes() ||
        std::memcmp(
            scanline->data() + pixelOffsetInScanline,
            pixels[i].data(),
            expectedPixelSpecification.bytesPerPixel)) {
      return ::testing::AssertionFailure()
          << "the idx=" << i << " element does not match";
    }
  }

  return ::testing::AssertionSuccess();
}

::testing::AssertionResult assertScanlineGray(
    pixel::testutils::InitializerVector&& pixels,
    const image::Scanline* const scanline) {
  return assertScanline(
      std::move(pixels), image::pixel::specifications::Gray, scanline);
}

::testing::AssertionResult assertScanlineRgb(
    pixel::testutils::InitializerVector&& pixels,
    const image::Scanline* const scanline) {
  return assertScanline(
      std::move(pixels), image::pixel::specifications::RGB, scanline);
}

::testing::AssertionResult assertScanlineArgb(
    pixel::testutils::InitializerVector&& pixels,
    const image::Scanline* const scanline) {
  return assertScanline(
      std::move(pixels), image::pixel::specifications::ARGB, scanline);
}

::testing::AssertionResult assertScanlineRgba(
    pixel::testutils::InitializerVector&& pixels,
    const image::Scanline* const scanline) {
  return assertScanline(
      std::move(pixels), image::pixel::specifications::RGBA, scanline);
}

//
// assertColor
//

void assertScanlineIsColor(
    const image::Scanline& scanline,
    const image::pixel::Specification& expectedPixelSpecification,
    pixel::testutils::Initializer&& expectedPixel) {
  ASSERT_EQ(expectedPixelSpecification, scanline.specification());

  for (int i = 0; i < scanline.width(); i++) {
    ASSERT_EQ(
        0,
        std::memcmp(
            scanline.dataAtPixel(i),
            expectedPixel.data(),
            expectedPixel.size()));
  }
}

void assertScanlineIsColorGray(
    const image::Scanline& scanline,
    pixel::testutils::Initializer&& expectedPixel) {
  assertScanlineIsColor(
      scanline, image::pixel::specifications::Gray, std::move(expectedPixel));
}

void assertScanlineIsColorRgb(
    const image::Scanline& scanline,
    pixel::testutils::Initializer&& expectedPixel) {
  assertScanlineIsColor(
      scanline, image::pixel::specifications::RGB, std::move(expectedPixel));
}

void assertScanlineIsColorRgba(
    const image::Scanline& scanline,
    pixel::testutils::Initializer&& expectedPixel) {
  assertScanlineIsColor(
      scanline, image::pixel::specifications::RGBA, std::move(expectedPixel));
}

void assertScanlineIsColorArgb(
    const image::Scanline& scanline,
    pixel::testutils::Initializer&& expectedPixel) {
  assertScanlineIsColor(
      scanline, image::pixel::specifications::ARGB, std::move(expectedPixel));
}

} // namespace testutils
} // namespace image
} // namespace spectrum
} // namespace facebook
