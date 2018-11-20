// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/requirements/Crop.h>
#include <spectrum/requirements/Resize.h>

#include <folly/Optional.h>

#include <algorithm>
#include <cmath>
#include <vector>

namespace facebook {
namespace spectrum {
namespace core {

/**
 * If the image can be brought to a size withing [0.75, 1.0] of the target size
 * just by sampling, we skip the scaling. This saves instructions, but more
 * important it avoids decreasing the image quality due to extra processing.
 */
constexpr float DEFAULT_THRESHOLD_FOR_SKIPPING_SCALING = 0.25;

enum class RESIZE_RATING : int {
  /**
   * Optimal resize decision (e.g. already target size or achievable using
   * sampling)
   */
  OPTIMAL = 0,

  /**
   * Good resize decision that is achieved without scaling (e.g. sampling
   * results in image within target boundaries). The secondary rating value will
   * increase with distance from target size.
   */
  GOOD_WITHOUT_SCALING = 1,

  /**
   * Good resize decision that requires scaling. The secondary rating value will
   * increase with amount of scaling necessary.
   */
  GOOD_WITH_SCALING = 2,

  /**
   * The image is much lower than the target resolution. This might be the case
   * if the cropped area results in an area that is much smaller than the target
   */
  BAD_TOO_SMALL = 3,

  /**
   * Invalid resize decision (e.g. the sampling results in a too small image
   * size)
   */
  INVALID = 10,
};

using ResizeDecisionRating = std::pair<RESIZE_RATING, float>;

class ResizeDecision {
 private:
  image::Size _inputSize;

  ResizeDecisionRating _rating = {RESIZE_RATING::INVALID, 0.0f};

  /** if set, then sampling should be applied */
  folly::Optional<image::Ratio> _samplingRatio;
  folly::Optional<image::Size> _sizeAfterSampling;

  /** if set, this cropping requirement should be applied after sampling */
  folly::Optional<requirements::Crop> _cropRequirement;
  folly::Optional<image::Size> _sizeAfterCropping;

  /** if set, then the scaling block shall resize to this target size */
  folly::Optional<image::Size> _sizeAfterScaling;

 public:
  explicit ResizeDecision(const image::Size& inputSize)
      : _inputSize(inputSize){};

  //
  // setters
  //

  ResizeDecision& sampling(
      const image::Ratio& samplingRatio,
      const image::Size& sizeAfterSampling);
  ResizeDecision& noSampling();

  ResizeDecision& cropping(
      const folly::Optional<requirements::Crop>& cropRequirement,
      const image::Size& sizeAfterCropping);
  ResizeDecision& noCropping();

  ResizeDecision& scaling(const image::Size& sizeAfterScaling);
  ResizeDecision& noScaling();

  ResizeDecision& rate(
      const RESIZE_RATING primaryRating,
      const float secondaryRating);
  ResizeDecision& rate(const RESIZE_RATING primaryRating);

  //
  // getters
  //

  bool shouldSample() const {
    return _samplingRatio.hasValue();
  }
  folly::Optional<image::Ratio> getSamplingRatio() const {
    return _samplingRatio;
  }

  /**
   * Returns the size after sampling has been applied
   */
  image::Size sizeAfterSampling() const;

  bool shouldCrop() const {
    return _cropRequirement.hasValue();
  }

  folly::Optional<requirements::Crop> cropRequirement() const {
    return _cropRequirement;
  }

  /**
   * Returns the size after sampling and cropping have been applied
   */
  image::Size sizeAfterCropping() const;

  bool shouldScale() const {
    return _sizeAfterScaling.hasValue();
  };

  /**
   * Returns the size after sampling, cropping and scaling have been
   * applied
   */
  image::Size sizeAfterScaling() const;

  ResizeDecisionRating getRating() const {
    return _rating;
  }

  bool operator==(const ResizeDecision& other) const;
};

/**
 * Calculates the proper resize decision for the given parameters. Where
 * feasible no-op operations will be returned (e.g. when the images is allowed
 * to be smaller and indeed is smaller). Alternatively, it is tried to use
 * sampling to resize the image to a size that's within:
 * [(1-thresholdForSkippingScaling) targetDimension, targetDimension].
 *
 * If these approaches do not work, a combination of scaling (if applicable) and
 * scaling is used.
 */
ResizeDecision calculateResizeDecision(
    const image::Size&,
    const folly::Optional<requirements::Resize>&,
    const std::vector<image::Ratio>& = {},
    const folly::Optional<requirements::Crop>& = folly::none,
    const float thresholdForSkippingScaling =
        DEFAULT_THRESHOLD_FOR_SKIPPING_SCALING);

} // namespace core
} // namespace spectrum
} // namespace facebook
