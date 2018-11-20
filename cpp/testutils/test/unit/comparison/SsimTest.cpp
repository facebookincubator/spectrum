// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/testutils/comparison/Ssim.h>
#include <spectrum/testutils/comparison/SsimTestUtils.h>

namespace facebook {
namespace spectrum {
namespace comparison {
namespace testutils {
namespace test {

TEST(comparison_testutils_compareSsim, whenImageTooSmall_thenFail) {
  const auto& graySpec = image::pixel::specifications::Gray;
  // h = 0, w = 0
  const auto imgEmpty1 = std::vector<std::unique_ptr<image::Scanline>>{};
  // h = 1, w = 0
  std::vector<std::unique_ptr<image::Scanline>> imgEmpty2{};
  imgEmpty2.push_back(std::make_unique<image::Scanline>(graySpec, 0));

  const auto img2x2 = makeImageFill(graySpec, {2, 2}, {0x99});
  const auto img3x8 = makeImageFill(graySpec, {3, 8}, {0x99});
  const auto img8x3 = makeImageFill(graySpec, {8, 3}, {0x99});
  const auto img8x8 = makeImageFill(graySpec, {8, 8}, {0x99});

  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, imgEmpty1, imgEmpty1), error::ImageTooSmall);
  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, imgEmpty2, imgEmpty2), error::ImageTooSmall);
  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, img2x2, img2x2), error::ImageTooSmall);
  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, img3x8, img3x8), error::ImageTooSmall);
  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, img8x3, img8x3), error::ImageTooSmall);

  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, img3x8, img8x3), error::ImageTooSmall);
  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, img8x3, img3x8), error::ImageTooSmall);

  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, img3x8, img8x8), error::ImageTooSmall);
  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, img8x8, img3x8), error::ImageSizesDontMatch);
}

TEST(comparison_testutils_compareSsim, whenImageSizeNotMatch_thenFail) {
  Rng rng{1234};
  const auto graySpec = image::pixel::specifications::Gray;
  const auto img12x34 = sampleImage(rng, graySpec, {12, 34});
  const auto img43x21 = sampleImage(rng, graySpec, {43, 21});
  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, img12x34, img43x21), error::ImageSizesDontMatch);
}

TEST(comparison_testutils_compareSsim, whenImageSame_thenReturnOne) {
  Rng rng{1235};
  const auto graySpec = image::pixel::specifications::Gray;
  const auto img8x8 = sampleImage(rng, graySpec, {8, 8});
  const auto img12x34 = sampleImage(rng, graySpec, {12, 34});
  EXPECT_FLOAT_EQ(1.0f, compareSsim(graySpec, img8x8, img8x8));
  EXPECT_FLOAT_EQ(1.0f, compareSsim(graySpec, img12x34, img12x34));
}

TEST(comparison_testutils_compareSsim, whenSpecsSame_thenSucceed) {
  Rng rng{1236};

  std::vector<image::pixel::Specification> specs = {
      image::pixel::specifications::Gray,
      image::pixel::specifications::AGray,
      image::pixel::specifications::GrayA,
      image::pixel::specifications::RGB,
      image::pixel::specifications::RGBA,
      image::pixel::specifications::ARGB};

  for (const auto& spec : specs) {
    const auto img8x8 = sampleImage(rng, spec, {8, 8});
    EXPECT_FLOAT_EQ(1.0f, compareSsim(spec, img8x8, img8x8));
  }
}

TEST(comparison_testutils_compareSsim, whenSpecsDifferent_thenFail) {
  Rng rng{1237};

  const auto graySpec = image::pixel::specifications::Gray;
  const auto rgbSpec = image::pixel::specifications::RGB;
  const auto argbSpec = image::pixel::specifications::ARGB;

  const auto imgGray = sampleImage(rng, graySpec, {8, 8});
  const auto imgRGB = sampleImage(rng, rgbSpec, {8, 8});
  const auto imgARGB = sampleImage(rng, argbSpec, {8, 8});

  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, imgRGB, imgRGB), error::ImageWrongSpecification);
  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, imgGray, imgRGB), error::ImageWrongSpecification);
  ASSERT_SPECTRUM_THROW(
      compareSsim(graySpec, imgRGB, imgGray), error::ImageWrongSpecification);

  ASSERT_SPECTRUM_THROW(
      compareSsim(rgbSpec, imgGray, imgGray), error::ImageWrongSpecification);
  ASSERT_SPECTRUM_THROW(
      compareSsim(rgbSpec, imgGray, imgRGB), error::ImageWrongSpecification);
  ASSERT_SPECTRUM_THROW(
      compareSsim(rgbSpec, imgRGB, imgGray), error::ImageWrongSpecification);

  ASSERT_SPECTRUM_THROW(
      compareSsim(rgbSpec, imgRGB, imgARGB), error::ImageWrongSpecification);
}

TEST(comparison_testutils_compareSsim, whenImageParamsSwapped_thenReturnSame) {
  Rng rng{1238};
  const auto& graySpec = image::pixel::specifications::Gray;

  const auto img1 = sampleImage(rng, graySpec, {22, 33});
  const auto img2 = sampleImage(rng, graySpec, {22, 33});
  const auto diff12 = compareSsim(graySpec, img1, img2);
  const auto diff21 = compareSsim(graySpec, img2, img1);

  EXPECT_FLOAT_EQ(diff12, diff21);
}

TEST(comparison_testutils_compareSsim, whenRunOnManySizes_thenSucceed) {
  const auto& graySpec = image::pixel::specifications::Gray;

  for (std::uint32_t width = 8; width < 16; width += 1) {
    for (std::uint32_t height = 8; height < 16; height += 1) {
      const auto img = makeImageFill(graySpec, {width, height}, {0xFF});
      EXPECT_NO_THROW(compareSsim(graySpec, img, img));
    }
  }
}

TEST(comparison_testutils_compareSsim, whenImagesDifferent_thenNotReturnOne) {
  Rng rng{1239};
  const auto& graySpec = image::pixel::specifications::Gray;

  const auto imgBlack = makeImageFill(graySpec, {8, 8}, {0x00});
  const auto imgWhite = makeImageFill(graySpec, {8, 8}, {0xFF});
  const auto imgRand1 = sampleImage(rng, graySpec, {8, 8});
  const auto imgRand2 = sampleImage(rng, graySpec, {8, 8});
  const auto diffWB = compareSsim(graySpec, imgWhite, imgBlack);
  const auto diffWR = compareSsim(graySpec, imgWhite, imgRand1);
  const auto diffBR = compareSsim(graySpec, imgBlack, imgRand1);
  const auto diffRR = compareSsim(graySpec, imgRand1, imgRand2);

  EXPECT_LT(diffWB, 0.1f);
  EXPECT_LT(diffWR, 0.1f);
  EXPECT_LT(diffBR, 0.1f);
  EXPECT_LT(diffRR, 0.5f);
}

TEST(
    comparison_testutils_compareSsim,
    whenRunOnSampleImage_thenReturnSameAsJavaImpl) {
  const auto img85x128_q85 =
      readJpegImage(testdata::paths::jpeg::s85x128_Q85.normalized());
  const auto img85x128_q15 =
      readJpegImage(testdata::paths::jpeg::s85x128_Q15.normalized());

  const auto height = img85x128_q85.size();
  EXPECT_EQ(height, 128);
  const auto width = img85x128_q85[0]->width();
  EXPECT_EQ(width, 85);
  const auto spec = img85x128_q85[0]->specification();
  EXPECT_EQ(spec, image::pixel::specifications::RGB);

  // Compare their ssim and ensure it matches the Java implementation
  const auto diff = compareSsim(spec, img85x128_q85, img85x128_q15);
  EXPECT_NEAR(diff, 0.81f, 0.03f);
}

} // namespace test
} // namespace testutils
} // namespace comparison
} // namespace spectrum
} // namespace facebook
