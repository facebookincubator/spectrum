// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/proc/ScanlineConversion.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace proc {
namespace test {

namespace {

// testing with more than one pixel to verify if we iterate correctly
constexpr std::size_t pixelRepetitions = 3;

inline void _testScanlineConversion(
    const image::pixel::Specification& inputSpecification,
    const image::pixel::Specification& outputSpecification,
    const std::vector<std::uint8_t> inputPixelData,
    const std::vector<std::uint8_t> expectedOutputPixelData,
    const image::Color& background = image::Color{0, 0, 0},
    const std::size_t numPixelsInPixelData = 1) {
  ASSERT_EQ(
      inputPixelData.size(),
      numPixelsInPixelData * inputSpecification.bytesPerPixel);
  ASSERT_EQ(
      expectedOutputPixelData.size(),
      numPixelsInPixelData * outputSpecification.bytesPerPixel);

  const auto scanlineWidth = numPixelsInPixelData * pixelRepetitions;

  // initialize and fill the scanline with repeated input pixel data
  auto inputScanline =
      std::make_unique<image::Scanline>(inputSpecification, scanlineWidth);
  const auto inputPixelLength = inputPixelData.size();
  for (std::size_t i = 0; i < pixelRepetitions; i++) {
    for (std::size_t j = 0; j < inputPixelLength; j++) {
      inputScanline->data()[i * inputPixelLength + j] = inputPixelData[j];
    }
  }

  const auto scanlineConverter = core::proc::makeScanlineConverter(
      inputSpecification, outputSpecification, background);
  const auto outputScanline =
      scanlineConverter->convertScanline(std::move(inputScanline));

  ASSERT_EQ(scanlineWidth, outputScanline->width());

  // assert the output scanline against repeated expected output pixel data
  const std::size_t outputPixelLength = expectedOutputPixelData.size();
  for (std::size_t i = 0; i < pixelRepetitions; i++) {
    for (std::size_t j = 0; j < outputPixelLength; j++) {
      const auto actual = outputScanline->data()[i * outputPixelLength + j];
      const auto expected = expectedOutputPixelData[j];
      ASSERT_EQ(actual, expected);
    }
  }
}

void assertRgbIndices(
    const image::pixel::Specification& specification,
    const std::uint8_t red,
    const std::uint8_t green,
    const std::uint8_t blue,
    const folly::Optional<std::uint8_t>& alpha) {
  const auto indices = indices::RGB{specification};

  ASSERT_EQ(red, indices.red);
  ASSERT_EQ(green, indices.green);
  ASSERT_EQ(blue, indices.blue);
  ASSERT_EQ(alpha.hasValue(), indices.hasAlpha);

  if (alpha.hasValue()) {
    ASSERT_EQ(*alpha, indices.alpha);
  }
}

void assertGrayIndices(
    const image::pixel::Specification& specification,
    const std::uint8_t gray,
    const folly::Optional<std::uint8_t>& alpha) {
  const auto indices = indices::Gray{specification};

  ASSERT_EQ(gray, indices.gray);
  ASSERT_EQ(alpha.hasValue(), indices.hasAlpha);

  if (alpha.hasValue()) {
    ASSERT_EQ(*alpha, indices.alpha);
  }
}
} // namespace

//
// From Gray
//

TEST(core_proc_ScanlineConverter, whenGrayToGray_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::Gray,
      image::pixel::specifications::Gray,
      {10},
      {10});
}

TEST(core_proc_ScanlineConverter, whenGrayToRgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::Gray,
      image::pixel::specifications::RGB,
      {10},
      {10, 10, 10});
}

TEST(core_proc_ScanlineConverter, whenGrayToArgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::Gray,
      image::pixel::specifications::ARGB,
      {10},
      {255, 10, 10, 10});
}

TEST(core_proc_ScanlineConverter, whenGrayToRgba_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::Gray,
      image::pixel::specifications::RGBA,
      {10},
      {10, 10, 10, 255});
}

//
// From RGB
//

TEST(core_proc_ScanlineConverter, whenRgbToGray_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGB,
      image::pixel::specifications::Gray,
      {10, 10, 40},
      {20});
}

TEST(core_proc_ScanlineConverter, whenRgbToRgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGB,
      image::pixel::specifications::RGB,
      {1, 2, 3},
      {1, 2, 3});
}

TEST(core_proc_ScanlineConverter, whenRgbToArgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGB,
      image::pixel::specifications::ARGB,
      {1, 2, 3},
      {255, 1, 2, 3});
}

TEST(core_proc_ScanlineConverter, whenRgbToRgba_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGB,
      image::pixel::specifications::RGBA,
      {1, 2, 3},
      {1, 2, 3, 255});
}

//
// From ARGB
//

TEST(core_proc_ScanlineConverter, whenArgbToGray_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::ARGB,
      image::pixel::specifications::Gray,
      {128, 10, 20, 30},
      {10});
}

TEST(core_proc_ScanlineConverter, whenArgbToRgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::ARGB,
      image::pixel::specifications::RGB,
      {128, 10, 20, 30},
      {5, 10, 15});
}

TEST(core_proc_ScanlineConverter, whenArgbToArgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::ARGB,
      image::pixel::specifications::ARGB,
      {128, 10, 20, 30},
      {128, 10, 20, 30});
}

TEST(core_proc_ScanlineConverter, whenArgbToRgba_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::ARGB,
      image::pixel::specifications::RGBA,
      {128, 10, 20, 30},
      {10, 20, 30, 128});
}

//
// From XRGB
//

TEST(core_proc_ScanlineConverter, whenXrgbToGray_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::XRGB,
      image::pixel::specifications::Gray,
      {128, 10, 20, 30},
      {20});
}

TEST(core_proc_ScanlineConverter, whenXrgbToRgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::XRGB,
      image::pixel::specifications::RGB,
      {128, 10, 20, 30},
      {10, 20, 30});
}

TEST(core_proc_ScanlineConverter, whenXrgbToArgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::XRGB,
      image::pixel::specifications::ARGB,
      {128, 10, 20, 30},
      {255, 10, 20, 30});
}

TEST(core_proc_ScanlineConverter, whenXrgbToRgba_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::XRGB,
      image::pixel::specifications::RGBA,
      {128, 10, 20, 30},
      {10, 20, 30, 255});
}

//
// From RGBA
//

TEST(core_proc_ScanlineConverter, whenRgbaToGray_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGBA,
      image::pixel::specifications::Gray,
      {10, 20, 30, 128},
      {10});
}

TEST(core_proc_ScanlineConverter, whenRgbaToRgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGBA,
      image::pixel::specifications::RGB,
      {10, 20, 30, 128},
      {5, 10, 15});
}

TEST(core_proc_ScanlineConverter, whenRgbaToArgb_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGBA,
      image::pixel::specifications::ARGB,
      {10, 20, 30, 128},
      {128, 10, 20, 30});
}

TEST(core_proc_ScanlineConverter, whenRgbaToRgba_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGBA,
      image::pixel::specifications::RGBA,
      {10, 20, 30, 128},
      {10, 20, 30, 128});
}

//
// Test multiple pixels
//

TEST(core_proc_ScanlineConverter, whenMultiplePixels_rgbToGray) {
  _testScanlineConversion(
      image::pixel::specifications::RGB,
      image::pixel::specifications::Gray,
      {10, 10, 40, 10, 10, 40, 10, 10, 40},
      {20, 20, 20},
      image::Color{0, 0, 0},
      3);
}

TEST(
    core_proc_ScanlineConverter,
    whenMultiplePixels_rgbaToRgbAndHalfAlpha_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGBA,
      image::pixel::specifications::RGB,
      {10, 20, 30, 128, 10, 20, 30, 128, 10, 20, 30, 128},
      {14, 29, 44, 14, 29, 44, 14, 29, 44}, // integer rounding error
      image::Color{20, 40, 60},
      3);
}

//
// Test background color
//

TEST(
    core_proc_ScanlineConverter,
    whenRgbaToRgbWithBackground_alphaNone_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGBA,
      image::pixel::specifications::RGB,
      {10, 20, 30, 0},
      {20, 40, 60}, // integer rounding error
      image::Color{20, 40, 60});
}

TEST(
    core_proc_ScanlineConverter,
    whenRgbaToRgbWithBackground_alphaHalf_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGBA,
      image::pixel::specifications::RGB,
      {10, 20, 30, 128},
      {14, 29, 44}, // integer rounding error
      image::Color{20, 40, 60});
}

TEST(
    core_proc_ScanlineConverter,
    whenRgbaToRgbWithBackground_alphaFull_thenCorrect) {
  _testScanlineConversion(
      image::pixel::specifications::RGBA,
      image::pixel::specifications::RGB,
      {10, 20, 30, 255},
      {10, 20, 30}, // integer rounding error
      image::Color{20, 40, 60});
}

TEST(core_proc_indices_RGB, whenGeneratingRGBAIndices_thenCorrect) {
  assertRgbIndices(image::pixel::specifications::RGBA, 0, 1, 2, 3);
}

TEST(core_proc_indices_RGB, whenGeneratingRGBXIndices_thenCorrect) {
  assertRgbIndices(image::pixel::specifications::RGBX, 0, 1, 2, folly::none);
}

TEST(core_proc_indices_RGB, whenGeneratingARGBIndices_thenCorrect) {
  assertRgbIndices(image::pixel::specifications::ARGB, 1, 2, 3, 0);
}

TEST(core_proc_indices_RGB, whenGeneratingXRGBIndices_thenCorrect) {
  assertRgbIndices(image::pixel::specifications::XRGB, 1, 2, 3, folly::none);
}

TEST(core_proc_indices_RGB, whenGeneratingRGBIndices_thenCorrect) {
  assertRgbIndices(image::pixel::specifications::RGB, 0, 1, 2, folly::none);
}

TEST(core_proc_indices_RGB, whenGeneratingBGRIndices_thenCorrect) {
  assertRgbIndices(image::pixel::specifications::BGR, 2, 1, 0, folly::none);
}

TEST(core_proc_indices_Gray, whenGeneratingGrayIndices_thenCorrect) {
  assertGrayIndices(image::pixel::specifications::Gray, 0, folly::none);
}

TEST(core_proc_indices_Gray, whenGeneratingAGrayIndices_thenCorrect) {
  const auto aGray = image::pixel::specifications::AGray;
  const auto reversedAGray = image::pixel::Specification(
      aGray.colorModel,
      aGray.bytesPerPixel,
      image::pixel::AlphaInfo::Last,
      image::pixel::ComponentsOrder::Reversed);

  assertGrayIndices(aGray, 1, 0);
  assertGrayIndices(reversedAGray, 1, 0);
}

TEST(core_proc_indices_Gray, whenGeneratingGrayAIndices_thenCorrect) {
  const auto grayA = image::pixel::specifications::GrayA;
  const auto reversedGrayA = image::pixel::Specification(
      grayA.colorModel,
      grayA.bytesPerPixel,
      image::pixel::AlphaInfo::First,
      image::pixel::ComponentsOrder::Reversed);

  assertGrayIndices(grayA, 0, 1);
  assertGrayIndices(reversedGrayA, 0, 1);
}

} // namespace test
} // namespace proc
} // namespace core
} // namespace spectrum
} // namespace facebook
