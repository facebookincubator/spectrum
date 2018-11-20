// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Recipe.h>
#include <spectrum/Rule.h>
#include <spectrum/Spectrum.h>

#include <folly/Optional.h>

#include <array>
#include <cstddef>
#include <memory>

namespace facebook {
namespace spectrum {
namespace testutils {

//
// Recipe & Recipe::Factory
//

/**
 * Creates a test transcode recipe that will copy the entire source to the sink.
 */
Recipe::Factory makeLambdaFactoryForCopyRecipe();

/**
 * Test transcode recipe that will just output the given string and ignore the
 * image source. Output image information will match: JPEG, 320x240, RGB_8
 */
class ConstantOutputRecipe : public Recipe {
 private:
  std::string output;

 public:
  explicit ConstantOutputRecipe(const std::string& output);

  image::Specification perform(const core::Operation& operation) const override;
};

Recipe::Factory makeLambdaFactoryForConstantOutputRecipe(std::string output);

Recipe::Factory makeLambdaFactoryForNullRecipe();

Spectrum makeSpectrumWithPlugin(Plugin&& plugin);

Rule makeRule(
    const std::string& ruleName,
    const image::Format& acceptedInputFormat = image::formats::Jpeg,
    const image::Format& acceptedOutputFormat = image::formats::Jpeg,
    folly::Optional<Recipe::Factory>&& recipeFactory = folly::none);

TranscodeOptions makeDummyTranscodeOptions();

} // namespace testutils
} // namespace spectrum
} // namespace facebook
