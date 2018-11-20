// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/testutils/comparison/Ssim.h>

constexpr float ssimK1 = 0.01f;
constexpr float ssimK2 = 0.03f;
constexpr float ssimMaxValue = 256;

constexpr float ssimC1 = (ssimK1 * ssimMaxValue) * (ssimK1 * ssimMaxValue);
constexpr float ssimC2 = (ssimK2 * ssimMaxValue) * (ssimK2 * ssimMaxValue);

constexpr std::size_t ssimWindowSize = 8;
constexpr std::size_t ssimWindowStep = 4;
constexpr std::size_t ssimWindowArea = ssimWindowSize * ssimWindowSize;
constexpr std::size_t ssimWindowAreaSq = ssimWindowArea * ssimWindowArea;

constexpr std::size_t ssimMaxChannels = 5;

namespace facebook {
namespace spectrum {
namespace comparison {
namespace testutils {

namespace error {
const folly::StringPiece ImageTooSmall{"ssim_image_too_small"};
const folly::StringPiece ImageSizesDontMatch{"ssim_image_sizes_dont_match"};
const folly::StringPiece ImageWrongSpecification{
    "ssim_image_wrong_specification"};
} // namespace error

namespace /* anonymous */ {
/**
 * Compute the mean SSIM of all channels in the given window
 */
inline float ssimWindowMean(
    const image::pixel::Specification& spec,
    const std::vector<std::unique_ptr<image::Scanline>>& imgA,
    const std::vector<std::unique_ptr<image::Scanline>>& imgB,
    const int x,
    const int y) {
  // compute SSIM over all channels
  const auto numChannels = spec.numberOfComponents();

  // sumsA/B = sum of pixels in window by channel
  std::array<int, ssimMaxChannels> sumsA{};
  std::array<int, ssimMaxChannels> sumsB{};
  // sumSqsA/B = sum of pixels^2 in window by channel
  std::array<int, ssimMaxChannels> sumSqsA{};
  std::array<int, ssimMaxChannels> sumSqsB{};
  // sumProdAB = sum of pixel_A * pixel_B in window by channel
  std::array<int, ssimMaxChannels> sumProdAB{};

  // compute sumsA/B / sumSqsA/B / sumProdAB
  for (int i = y; i < y + ssimWindowSize; i += 1) {
    const auto* rowA = imgA[i].get();
    const auto* rowB = imgB[i].get();

    for (int j = x; j < x + ssimWindowSize; j += 1) {
      const auto* pixelA = rowA->dataAtPixel(j);
      const auto* pixelB = rowB->dataAtPixel(j);

      for (int c = 0; c < numChannels; c += 1) {
        const auto chanA = pixelA[c];
        const auto chanB = pixelB[c];
        sumsA[c] += chanA;
        sumsB[c] += chanB;
        sumSqsA[c] += chanA * chanA;
        sumSqsB[c] += chanB * chanB;
        sumProdAB[c] += chanA * chanB;
      }
    }
  }

  // the mean SSIM of all channels in the window
  float meanSsim = 0.0f;

  // compute channel SSIMs and fold into the mean SSIM
  for (int c = 0; c < numChannels; c += 1) {
    // compute channel means, variances, and covariance
    const float meanA = float(sumsA[c]) / ssimWindowArea;
    const float meanB = float(sumsB[c]) / ssimWindowArea;
    const float varA = float(sumSqsA[c]) / ssimWindowArea -
        float(sumsA[c] * sumsA[c]) / ssimWindowAreaSq;
    const float varB = float(sumSqsB[c]) / ssimWindowArea -
        float(sumsB[c] * sumsB[c]) / ssimWindowAreaSq;
    const float covAB = float(sumProdAB[c]) / ssimWindowArea -
        float(sumsA[c] * sumsB[c]) / ssimWindowAreaSq;

    // compute SSIM of the channel
    const float t1 = 2 * meanA * meanB + ssimC1;
    const float t2 = 2 * covAB + ssimC2;
    const float t3 = meanA * meanA + meanB * meanB + ssimC1;
    const float t4 = varA + varB + ssimC2;
    const float ssim = (t1 * t2) / (t3 * t4);

    // numerically stable mean
    // mu_n = (\sum_{i=1}^n x_i) / n
    //      = mu_{n-1} + (x_n + mu_{n-1}) / n
    meanSsim = meanSsim + (ssim - meanSsim) / (c + 1);
  }

  return meanSsim;
}
} // namespace

float compareSsim(
    const image::pixel::Specification& spec,
    const std::vector<std::unique_ptr<image::Scanline>>& imgA,
    const std::vector<std::unique_ptr<image::Scanline>>& imgB) {
  const auto height = imgA.size();
  SPECTRUM_ERROR_IF(height < ssimWindowSize, error::ImageTooSmall);

  const auto width = imgA[0]->width();
  SPECTRUM_ERROR_IF(width < ssimWindowSize, error::ImageTooSmall);

  // validate images
  SPECTRUM_ERROR_IF(height != imgB.size(), error::ImageSizesDontMatch);
  for (int y = 0; y < height; y += 1) {
    SPECTRUM_ERROR_IF(imgA[y]->width() != width, error::ImageSizesDontMatch);
    SPECTRUM_ERROR_IF(imgB[y]->width() != width, error::ImageSizesDontMatch);
    SPECTRUM_ERROR_IF(
        imgA[y]->specification() != spec, error::ImageWrongSpecification);
    SPECTRUM_ERROR_IF(
        imgB[y]->specification() != spec, error::ImageWrongSpecification);
  }

  // the mean SSIM of all windows in the image
  float meanSsim = 0.0f;
  int run = 1;

  // compute the mean SSIM
  for (int y = 0; y <= height - ssimWindowSize; y += ssimWindowStep) {
    for (int x = 0; x <= width - ssimWindowSize; x += ssimWindowStep) {
      const float windowSsim = ssimWindowMean(spec, imgA, imgB, x, y);

      // numerically stable mean
      meanSsim = meanSsim + (windowSsim - meanSsim) / run;
      run += 1;
    }
  }

  return meanSsim;
}

} // namespace testutils
} // namespace comparison
} // namespace spectrum
} // namespace facebook
