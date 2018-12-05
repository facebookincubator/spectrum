// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/core/ResizeUtil.h>
#include <spectrum/requirements/CropAbsoluteToOrigin.h>
#include <spectrum/requirements/CropRelativeToOrigin.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace test {

//
// ResizeDecision
//

TEST(ResizeDecision, whenCreated_thenNoOp) {
  const ResizeDecision rd = ResizeDecision(image::Size{480, 320});

  ASSERT_EQ(false, rd.shouldSample());
  ASSERT_EQ(folly::none, rd.getSamplingRatio());
  ASSERT_EQ((image::Size{480, 320}), rd.sizeAfterSampling());

  ASSERT_EQ(false, rd.shouldCrop());
  ASSERT_FALSE(rd.cropRequirement().hasValue());
  ASSERT_EQ((image::Size{480, 320}), rd.sizeAfterCropping());

  ASSERT_EQ(false, rd.shouldScale());
  ASSERT_EQ((image::Size{480, 320}), rd.sizeAfterScaling());

  ASSERT_EQ(std::make_pair(RESIZE_RATING::INVALID, 0.0f), rd.getRating());
}

TEST(
    ResizeDecision,
    whenCreatedAndSampling_thenSamplingReflectedInFollowingSteps) {
  const ResizeDecision rd =
      ResizeDecision(image::Size{480, 320})
          .sampling(image::Ratio{1, 2}, image::Size{240, 160});

  ASSERT_EQ(true, rd.shouldSample());
  ASSERT_EQ((image::Ratio{1, 2}), rd.getSamplingRatio());
  ASSERT_EQ((image::Size{240, 160}), rd.sizeAfterSampling());
  ASSERT_EQ((image::Size{240, 160}), rd.sizeAfterCropping());
  ASSERT_EQ((image::Size{240, 160}), rd.sizeAfterScaling());

  ASSERT_EQ(std::make_pair(RESIZE_RATING::INVALID, 0.0f), rd.getRating());
}

TEST(
    ResizeDecision,
    whenCreatedAndCropping_thenCroppingReflectedInFollowingSteps) {
  auto cropRequirement = requirements::CropAbsoluteToOrigin(
      {.top = 10, .left = 10, .bottom = 310, .right = 470}, true);

  const ResizeDecision rd =
      ResizeDecision(image::Size{480, 320})
          .cropping(cropRequirement, image::Size{460, 300});

  ASSERT_EQ((image::Size{480, 320}), rd.sizeAfterSampling());
  ASSERT_EQ(true, rd.shouldCrop());
  ASSERT_EQ(cropRequirement, rd.cropRequirement());
  ASSERT_EQ((image::Size{460, 300}), rd.sizeAfterCropping());
  ASSERT_EQ((image::Size{460, 300}), rd.sizeAfterScaling());

  ASSERT_EQ(std::make_pair(RESIZE_RATING::INVALID, 0.0f), rd.getRating());
}

TEST(
    ResizeDecision,
    whenCreatedAndScaling_thenScallingReflectedInFollowingSteps) {
  const ResizeDecision rd =
      ResizeDecision(image::Size{480, 320}).scaling(image::Size{240, 160});

  ASSERT_EQ((image::Size{480, 320}), rd.sizeAfterSampling());
  ASSERT_EQ((image::Size{480, 320}), rd.sizeAfterCropping());
  ASSERT_EQ(true, rd.shouldScale());
  ASSERT_EQ((image::Size{240, 160}), rd.sizeAfterScaling());

  ASSERT_EQ(std::make_pair(RESIZE_RATING::INVALID, 0.0f), rd.getRating());
}

TEST(
    ResizeDecision,
    whenCreatedAndSamplingAndCroppingAndScaling_thenAllReflected) {
  const auto cropRequirement = requirements::CropAbsoluteToOrigin(
      {.top = 10, .left = 10, .bottom = 310, .right = 470}, true);

  const ResizeDecision rd =
      ResizeDecision(image::Size{960, 640})
          .sampling(image::Ratio{1, 2}, image::Size{480, 320})
          .cropping(cropRequirement, image::Size{460, 300})
          .scaling(image::Size{230, 150});

  ASSERT_EQ(true, rd.shouldSample());
  ASSERT_EQ((image::Ratio{1, 2}), rd.getSamplingRatio());
  ASSERT_EQ((image::Size{480, 320}), rd.sizeAfterSampling());

  ASSERT_EQ(true, rd.shouldCrop());
  ASSERT_EQ(cropRequirement, rd.cropRequirement());
  ASSERT_EQ((image::Size{460, 300}), rd.sizeAfterCropping());

  ASSERT_EQ(true, rd.shouldScale());
  ASSERT_EQ((image::Size{230, 150}), rd.sizeAfterScaling());

  ASSERT_EQ(std::make_pair(RESIZE_RATING::INVALID, 0.0f), rd.getRating());
}

TEST(ResizeDecisionRating, whenRatingBetter_thenSortedAscending) {
  std::vector<ResizeDecisionRating> ratings;

  ratings.push_back(ResizeDecisionRating{RESIZE_RATING::INVALID, 0.0f});
  ratings.push_back(
      ResizeDecisionRating{RESIZE_RATING::GOOD_WITHOUT_SCALING, 20.0f});
  ratings.push_back(ResizeDecisionRating{RESIZE_RATING::OPTIMAL, 0.0f});
  ratings.push_back(
      ResizeDecisionRating{RESIZE_RATING::GOOD_WITH_SCALING, 10.0f});
  ratings.push_back(ResizeDecisionRating{RESIZE_RATING::INVALID, 0.0f});
  ratings.push_back(ResizeDecisionRating{RESIZE_RATING::OPTIMAL, 0.0f});
  ratings.push_back(
      ResizeDecisionRating{RESIZE_RATING::GOOD_WITHOUT_SCALING, 10.0f});
  ratings.push_back(
      ResizeDecisionRating{RESIZE_RATING::GOOD_WITH_SCALING, 20.0f});
  ratings.push_back(ResizeDecisionRating{RESIZE_RATING::INVALID, 0.0f});

  std::sort(ratings.begin(), ratings.end());

  ASSERT_EQ(std::make_pair(RESIZE_RATING::INVALID, 0.0f), ratings.back());
  ratings.pop_back();
  ASSERT_EQ(std::make_pair(RESIZE_RATING::INVALID, 0.0f), ratings.back());
  ratings.pop_back();
  ASSERT_EQ(std::make_pair(RESIZE_RATING::INVALID, 0.0f), ratings.back());
  ratings.pop_back();
  ASSERT_EQ(
      std::make_pair(RESIZE_RATING::GOOD_WITH_SCALING, 20.0f), ratings.back());
  ratings.pop_back();
  ASSERT_EQ(
      std::make_pair(RESIZE_RATING::GOOD_WITH_SCALING, 10.0f), ratings.back());
  ratings.pop_back();
  ASSERT_EQ(
      std::make_pair(RESIZE_RATING::GOOD_WITHOUT_SCALING, 20.0f),
      ratings.back());
  ratings.pop_back();
  ASSERT_EQ(
      std::make_pair(RESIZE_RATING::GOOD_WITHOUT_SCALING, 10.0f),
      ratings.back());
  ratings.pop_back();
  ASSERT_EQ(std::make_pair(RESIZE_RATING::OPTIMAL, 0.0f), ratings.back());
  ratings.pop_back();
  ASSERT_EQ(std::make_pair(RESIZE_RATING::OPTIMAL, 0.0f), ratings.back());
  ratings.pop_back();

  ASSERT_TRUE(ratings.empty());
}

//
// Logic tests
//

TEST(ResizeUtil, whenMatchesTarget_thenNoOp) {
  const image::Size inputSize = {200, 400};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::Exact,
      .targetSize = image::Size{200, 400},
  };

  const ResizeDecision actual =
      calculateResizeDecision(inputSize, resizeRequirement);

  ASSERT_EQ(false, actual.shouldSample());
  ASSERT_EQ(false, actual.shouldCrop());
  ASSERT_EQ(false, actual.shouldScale());
  ASSERT_EQ(RESIZE_RATING::OPTIMAL, actual.getRating().first);
}

TEST(ResizeUtil, whenSmallerThanTarget_andSmallerAllowed_thenNoOp) {
  const image::Size inputSize = {370, 380};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::ExactOrSmaller,
      .targetSize = image::Size{400, 400},
  };

  const ResizeDecision actual =
      calculateResizeDecision(inputSize, resizeRequirement);

  ASSERT_EQ(false, actual.shouldSample());
  ASSERT_EQ(false, actual.shouldCrop());
  ASSERT_EQ(false, actual.shouldScale());
  ASSERT_EQ(inputSize, actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::GOOD_WITHOUT_SCALING, actual.getRating().first);
}

TEST(
    ResizeUtil,
    whenSmallerThanTarget_andSmallerNotAllowed_thenMarkAsUnfulfillable) {
  const auto inputSize = image::Size{380, 380};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::Exact,
      .targetSize = image::Size{400, 400},
  };
  const auto actual = calculateResizeDecision(inputSize, resizeRequirement);

  ASSERT_EQ(inputSize, actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::BAD_TOO_SMALL, actual.getRating().first);
}

TEST(ResizeUtil, whenLargerThanTarget_andNoRatios_thenOnlyScaling) {
  const image::Size inputSize = {210, 420};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::ExactOrSmaller,
      .targetSize = image::Size{400, 400},
  };

  const ResizeDecision actual =
      calculateResizeDecision(inputSize, resizeRequirement);

  ASSERT_EQ(false, actual.shouldSample());
  ASSERT_EQ(false, actual.shouldCrop());
  ASSERT_EQ(true, actual.shouldScale());
  ASSERT_EQ((image::Size{200, 400}), actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::GOOD_WITH_SCALING, actual.getRating().first);
}

TEST(ResizeUtil, whenLargerThanTarget_andNoRatios_thenOnlyScaling_rounding) {
  const image::Size inputSize = {800, 530};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::ExactOrSmaller,
      .targetSize = image::Size{128, 128},
  };

  const ResizeDecision actual =
      calculateResizeDecision(inputSize, resizeRequirement);

  ASSERT_EQ(false, actual.shouldSample());
  ASSERT_EQ(false, actual.shouldCrop());
  ASSERT_EQ(true, actual.shouldScale());
  ASSERT_EQ((image::Size{128, 85}), actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::GOOD_WITH_SCALING, actual.getRating().first);
}

TEST(ResizeUtil, whenLargerThanTarget_andHasPerfectRatio_thenOnlySampling) {
  const image::Size inputSize = {800, 600};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::ExactOrSmaller,
      .targetSize = image::Size{400, 400},
  };
  const auto samplingRatios = std::vector<image::Ratio>{{1, 4}, {1, 2}, {2, 3}};

  const ResizeDecision actual =
      calculateResizeDecision(inputSize, resizeRequirement, samplingRatios);

  ASSERT_EQ(true, actual.shouldSample());
  ASSERT_EQ((image::Ratio{1, 2}), actual.getSamplingRatio());
  ASSERT_EQ((image::Size{400, 300}), actual.sizeAfterSampling());
  ASSERT_EQ(false, actual.shouldCrop());
  ASSERT_EQ(false, actual.shouldScale());
  ASSERT_EQ((image::Size{400, 300}), actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::GOOD_WITHOUT_SCALING, actual.getRating().first);
}

TEST(
    ResizeUtil,
    whenLargerThanTarget_andHasRatioWithinThreshold_thenOnlySampling) {
  const image::Size inputSize = {800, 600};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::ExactOrSmaller,
      .targetSize = image::Size{400, 400},
  };
  const auto samplingRatios =
      std::vector<image::Ratio>{{1, 100}, {49, 100}, {99, 100}};

  const ResizeDecision actual =
      calculateResizeDecision(inputSize, resizeRequirement, samplingRatios);

  ASSERT_EQ(true, actual.shouldSample());
  ASSERT_EQ((image::Ratio{49, 100}), actual.getSamplingRatio());
  ASSERT_EQ((image::Size{392, 294}), actual.sizeAfterSampling());
  ASSERT_EQ(false, actual.shouldCrop());
  ASSERT_EQ(false, actual.shouldScale());
  ASSERT_EQ((image::Size{392, 294}), actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::GOOD_WITHOUT_SCALING, actual.getRating().first);
}

TEST(
    ResizeUtil,
    whenLargerThanTarget_andHasRatioBelowThreshold_andNoUpperRatio_thenOnlyScaling) {
  const image::Size inputSize = {800, 600};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::ExactOrSmaller,
      .targetSize = image::Size{400, 400},
  };
  const auto samplingRatios =
      std::vector<image::Ratio>{{1, 100}, {2, 100}, {3, 100}};

  const ResizeDecision actual =
      calculateResizeDecision(inputSize, resizeRequirement, samplingRatios);

  ASSERT_EQ(false, actual.shouldSample());
  ASSERT_EQ(false, actual.shouldCrop());
  ASSERT_EQ(true, actual.shouldScale());
  ASSERT_EQ((image::Size{400, 300}), actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::GOOD_WITH_SCALING, actual.getRating().first);
}

TEST(
    ResizeUtil,
    whenLargerThanTarget_andHasRatioBelowThreshold_andHasUpperRatio_thenSamplingAndScaling) {
  const image::Size inputSize = {800, 600};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::ExactOrSmaller,
      .targetSize = image::Size{400, 400},
  };
  const auto samplingRatios =
      std::vector<image::Ratio>{{1, 100}, {2, 100}, {75, 100}, {102, 100}};

  const ResizeDecision actual =
      calculateResizeDecision(inputSize, resizeRequirement, samplingRatios);

  ASSERT_EQ(true, actual.shouldSample());
  ASSERT_EQ((image::Ratio{75, 100}), actual.getSamplingRatio());
  ASSERT_EQ((image::Size{600, 450}), actual.sizeAfterSampling());
  ASSERT_EQ(false, actual.shouldCrop());
  ASSERT_EQ(true, actual.shouldScale());
  ASSERT_EQ((image::Size{400, 300}), actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::GOOD_WITH_SCALING, actual.getRating().first);
}

TEST(
    ResizeUtil,
    whenLargerThanTarget_andHasNoSmallerRatio_andHasUpperRatio_thenSamplingAndScaling) {
  const image::Size inputSize = {800, 600};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::ExactOrSmaller,
      .targetSize = image::Size{400, 400},
  };
  const auto samplingRatios = std::vector<image::Ratio>{{75, 100}, {102, 100}};

  const ResizeDecision actual =
      calculateResizeDecision(inputSize, resizeRequirement, samplingRatios);

  ASSERT_EQ(true, actual.shouldSample());
  ASSERT_EQ((image::Ratio{75, 100}), actual.getSamplingRatio());
  ASSERT_EQ((image::Size{600, 450}), actual.sizeAfterSampling());
  ASSERT_EQ(false, actual.shouldCrop());
  ASSERT_EQ(true, actual.shouldScale());
  ASSERT_EQ((image::Size{400, 300}), actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::GOOD_WITH_SCALING, actual.getRating().first);
}

TEST(ResizeUtil, whenNotResize_andJustCropping_thenJustCropping) {
  const auto inputSize = image::Size{1000, 800};
  const auto cropRequirement = requirements::CropRelativeToOrigin(
      {.top = 0.1f, .left = 0.1f, .bottom = 0.9f, .right = 0.9f}, false);

  const auto actual =
      calculateResizeDecision(inputSize, folly::none, {}, cropRequirement);

  ASSERT_EQ(false, actual.shouldSample());

  ASSERT_EQ(true, actual.shouldCrop());
  ASSERT_EQ((image::Size{800, 640}), actual.sizeAfterCropping());

  ASSERT_EQ(false, actual.shouldScale());
}

TEST(
    ResizeUtil,
    whenLargerThanTarget_andHasNoSmallerRatio_andHasUpperRatio_thenSamplingAndCroppingAndScaling) {
  const auto inputSize = image::Size{1000, 800};
  const auto resizeRequirement = requirements::Resize{
      .mode = requirements::Resize::Mode::ExactOrSmaller,
      .targetSize = image::Size{200, 200},
  };

  const auto cropRequirement = requirements::CropRelativeToOrigin(
      {.top = 0.1f, .left = 0.1f, .bottom = 0.9f, .right = 0.9f}, false);

  const auto samplingRatios = std::vector<image::Ratio>{{5, 10}};
  const auto actual = calculateResizeDecision(
      inputSize, resizeRequirement, samplingRatios, cropRequirement);

  ASSERT_EQ(true, actual.shouldSample());
  ASSERT_EQ((image::Ratio{5, 10}), actual.getSamplingRatio());
  ASSERT_EQ((image::Size{500, 400}), actual.sizeAfterSampling());

  ASSERT_EQ(true, actual.shouldCrop());
  ASSERT_EQ((image::Size{400, 320}), actual.sizeAfterCropping());

  ASSERT_EQ(true, actual.shouldScale());
  ASSERT_EQ((image::Size{200, 160}), actual.sizeAfterScaling());
  ASSERT_EQ(RESIZE_RATING::GOOD_WITH_SCALING, actual.getRating().first);
}

} // namespace test
} // namespace core
} // namespace spectrum
} // namespace facebook
