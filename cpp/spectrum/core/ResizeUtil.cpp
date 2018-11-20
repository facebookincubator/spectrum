// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ResizeUtil.h"

#include <cmath>
#include <vector>

namespace facebook {
namespace spectrum {
namespace core {

//
// ResizeDecision setters
//

ResizeDecision& ResizeDecision::sampling(
    const image::Ratio& samplingRatio,
    const image::Size& sizeAfterSampling) {
  this->_samplingRatio = samplingRatio;
  this->_sizeAfterSampling = sizeAfterSampling;
  return *this;
}

ResizeDecision& ResizeDecision::noSampling() {
  this->_sizeAfterSampling = folly::none;
  return *this;
}

ResizeDecision& ResizeDecision::cropping(
    const folly::Optional<requirements::Crop>& cropRequirement,
    const image::Size& sizeAfterCropping) {
  this->_cropRequirement = cropRequirement;
  this->_sizeAfterCropping = sizeAfterCropping;
  return *this;
}

ResizeDecision& ResizeDecision::noCropping() {
  this->_sizeAfterCropping = folly::none;
  return *this;
}

ResizeDecision& ResizeDecision::scaling(const image::Size& sizeAfterScaling) {
  this->_sizeAfterScaling = sizeAfterScaling;
  return *this;
}

ResizeDecision& ResizeDecision::noScaling() {
  this->_sizeAfterScaling = folly::none;
  return *this;
}

ResizeDecision& ResizeDecision::rate(
    const RESIZE_RATING primaryRating,
    const float secondaryRating) {
  this->_rating = {primaryRating, secondaryRating};
  return *this;
}

ResizeDecision& ResizeDecision::rate(const RESIZE_RATING primaryRating) {
  return rate(primaryRating, 0.0f);
}

//
// ResizeDecision getters
//

image::Size ResizeDecision::sizeAfterSampling() const {
  return _sizeAfterSampling.value_or(_inputSize);
}

image::Size ResizeDecision::sizeAfterCropping() const {
  return _sizeAfterCropping.value_or(sizeAfterSampling());
}

image::Size ResizeDecision::sizeAfterScaling() const {
  return _sizeAfterScaling.value_or(sizeAfterCropping());
}

bool ResizeDecision::operator==(const ResizeDecision& other) const {
  const auto l = std::tie(
      _rating,
      _samplingRatio,
      _sizeAfterSampling,
      _cropRequirement,
      _sizeAfterCropping,
      _sizeAfterScaling);
  const auto r = std::tie(
      other._rating,
      other._samplingRatio,
      other._sizeAfterSampling,
      other._cropRequirement,
      other._sizeAfterCropping,
      other._sizeAfterScaling);
  return l == r;
}

//
// Resize calculation logic
//

namespace {

/**
 * Resizes an optional cropping requirement given a sampling ratio
 */
folly::Optional<requirements::Crop> maybeSampleCropRequirement(
    const image::Ratio& samplingRatio,
    const folly::Optional<requirements::Crop>& cropRequirement) {
  if (!cropRequirement.hasValue() || samplingRatio.one()) {
    return cropRequirement;
  }
  return cropRequirement->scaled(samplingRatio);
}

/**
 * Calculates the resize decision given the `resizeRequirement`. The resulting
 * value in `rating` will determine whether it will be considered as a valid
 * solution. The order in the method follows the preference of the different
 * outcomes: OPTIMAL, GOOD_WITHOUT_SCALING, GOOD_WITH_SCALING, BAD_TOO_SMALL,
 * INVALID.
 */
ResizeDecision calculateResizeDecisionWithScaling(
    const image::Size& inputSize,
    const image::Size& sizeAfterCropping,
    const folly::Optional<requirements::Resize>& resizeRequirement,
    const float thresholdForSkippingScaling) {
  if (!resizeRequirement.hasValue()) {
    return ResizeDecision(inputSize).rate(
        RESIZE_RATING::GOOD_WITHOUT_SCALING, 0);
  } else {
    const auto targetSize = resizeRequirement->targetSize;

    // CASE 0: if matches perfectly, then return as optimal solution
    if (targetSize == sizeAfterCropping) {
      return ResizeDecision(inputSize).rate(RESIZE_RATING::OPTIMAL);
    }

    const auto isSmaller = targetSize >= sizeAfterCropping;
    const bool isLarger = !isSmaller;
    const float relativeDeltaToTargetWidth = std::abs(
        1.0f - ((float)sizeAfterCropping.width / (float)targetSize.width));
    const float relativeDeltaToTargetHeight = std::abs(
        1.0f - ((float)sizeAfterCropping.height / (float)targetSize.height));

    // used as a measure of how much too small the image is compared to its
    // container (i.e. the minimal amount to make it fill the container)
    const float relativeDeltaToTargetMin =
        std::min(relativeDeltaToTargetWidth, relativeDeltaToTargetHeight);
    const auto isWithinMinResizingthreshold =
        relativeDeltaToTargetMin <= thresholdForSkippingScaling;

    // CASE 1a: if matches within accepted boundaries, then return as a good
    // solution
    if (isSmaller &&
        resizeRequirement->mode == requirements::Resize::Mode::ExactOrSmaller &&
        isWithinMinResizingthreshold) {
      return ResizeDecision(inputSize).rate(
          RESIZE_RATING::GOOD_WITHOUT_SCALING, relativeDeltaToTargetMin);
    }

    // used as a measure of how much to large the image is compared to its
    // container (i.e. the minimal amount of scaling to make it fit within)
    const float relativeDeltaToTargetMax =
        std::max(relativeDeltaToTargetWidth, relativeDeltaToTargetHeight);

    // CASE 1b: if matches within accepted boundaries, then return as a good
    // solution
    if (isLarger &&
        resizeRequirement->mode == requirements::Resize::Mode::ExactOrLarger &&
        isWithinMinResizingthreshold) {
      return ResizeDecision(inputSize).rate(
          RESIZE_RATING::GOOD_WITHOUT_SCALING, relativeDeltaToTargetMin);
    }

    // CASE 2: if it is larger and requires scaling (and it is allowed) then
    // return as a good solution
    if (isLarger) {
      return ResizeDecision(inputSize)
          .scaling(sizeAfterCropping.downscaledToFit(targetSize))
          .rate(RESIZE_RATING::GOOD_WITH_SCALING, relativeDeltaToTargetMax);
    } else {
      return ResizeDecision(inputSize).rate(
          RESIZE_RATING::BAD_TOO_SMALL, relativeDeltaToTargetMin);
    }
  }
}

ResizeDecision calculateResizeDecisionWithCropping(
    const image::Size& inputSize,
    const image::Size& sizeAfterSampling,
    const folly::Optional<requirements::Resize>& resizeRequirement,
    const folly::Optional<requirements::Crop>& cropRequirement,
    const float thresholdForSkippingScaling) {
  if (!cropRequirement.hasValue()) {
    // if we don't crop, go ahead for scaling
    auto result = calculateResizeDecisionWithScaling(
        inputSize,
        sizeAfterSampling,
        resizeRequirement,
        thresholdForSkippingScaling);
    return result.noCropping();
  }
  // otherwise we need to update the input size
  const auto sizeAfterCropping = cropRequirement->apply(sizeAfterSampling).size;
  auto result = calculateResizeDecisionWithScaling(
      inputSize,
      sizeAfterCropping,
      resizeRequirement,
      thresholdForSkippingScaling);
  result.cropping(cropRequirement, sizeAfterCropping);
  return result;
}

ResizeDecision calculateResizeDecisionWithSampling(
    const image::Size& inputSize,
    const folly::Optional<image::Ratio>& samplingRatio,
    const folly::Optional<requirements::Crop>& cropRequirement,
    const folly::Optional<requirements::Resize>& resizeRequirement,
    const float thresholdForSkippingScaling) {
  if (!samplingRatio.has_value()) {
    // if we don't sample, go ahead for cropping
    auto result = calculateResizeDecisionWithCropping(
        inputSize,
        inputSize,
        resizeRequirement,
        cropRequirement,
        thresholdForSkippingScaling);
    return result.noSampling();
  }

  // otherwise we'll have to update the input size and the cropping
  // values
  auto sizeAfterSampling = image::sizeZero;
  try {
    sizeAfterSampling =
        inputSize.scaled(*samplingRatio, numeric::RoundingMode::Up);
  } catch (const SpectrumException&) {
    return ResizeDecision(inputSize).rate(RESIZE_RATING::INVALID);
  }
  const auto cropRequirementAfterSampling =
      maybeSampleCropRequirement(*samplingRatio, cropRequirement);

  auto result = calculateResizeDecisionWithCropping(
      inputSize,
      sizeAfterSampling,
      resizeRequirement,
      cropRequirementAfterSampling,
      thresholdForSkippingScaling);
  return result.sampling(samplingRatio.value(), sizeAfterSampling);
}
} // namespace

ResizeDecision calculateResizeDecision(
    const image::Size& inputSize,
    const folly::Optional<requirements::Resize>& resizeRequirement,
    const std::vector<image::Ratio>& samplingRatios,
    const folly::Optional<requirements::Crop>& cropRequirement,
    const float thresholdForSkippingScaling) {
  // quick return: if not allowed to resize, then return empty resize decision
  if (!resizeRequirement.hasValue()) {
    auto result = ResizeDecision(inputSize);
    if (cropRequirement.hasValue()) {
      result.cropping(cropRequirement, cropRequirement->apply(inputSize).size);
    }
    return result;
  }

  // Start with a variant that does not sample
  ResizeDecision bestResizeDecision = calculateResizeDecisionWithSampling(
      inputSize,
      folly::none,
      cropRequirement,
      resizeRequirement,
      thresholdForSkippingScaling);

  // then try the provided sampling ratios (if any)
  for (auto& ratio : samplingRatios) {
    // don't allow upscaling
    if (ratio.numerator > ratio.denominator) {
      continue;
    }

    const auto candidateResizeDecision = calculateResizeDecisionWithSampling(
        inputSize,
        ratio,
        cropRequirement,
        resizeRequirement,
        thresholdForSkippingScaling);

    // smaller values indicate better solutions
    if (candidateResizeDecision.getRating() < bestResizeDecision.getRating()) {
      bestResizeDecision = candidateResizeDecision;
    }
  }
  return bestResizeDecision;
}

} // namespace core
} // namespace spectrum
} // namespace facebook
