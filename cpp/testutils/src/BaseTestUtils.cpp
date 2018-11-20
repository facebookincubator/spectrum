// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "BaseTestUtils.h"

#include <spectrum/core/recipes/CopyRecipe.h>
#include <spectrum/testutils/image/SpecificationTestUtils.h>

namespace facebook {
namespace spectrum {
namespace testutils {

//
// Recipe & Recipe::Factory
//

Recipe::Factory makeLambdaFactoryForCopyRecipe() {
  return []() { return std::make_unique<core::recipes::CopyRecipe>(); };
}

ConstantOutputRecipe::ConstantOutputRecipe(const std::string& output)
    : output(std::move(output)) {}

image::Specification ConstantOutputRecipe::perform(
    const core::Operation& operation) const {
  operation.io.sink.write(output.data(), output.size());
  return image::Specification{
      .size = image::Size{320, 240},
      .format = image::formats::Jpeg,
      .pixelSpecification = image::pixel::specifications::RGB};
};

Recipe::Factory makeLambdaFactoryForConstantOutputRecipe(std::string output) {
  return [s = std::move(output)]() {
    return std::make_unique<ConstantOutputRecipe>(s);
  };
}

Recipe::Factory makeLambdaFactoryForNullRecipe() {
  return makeLambdaFactoryForConstantOutputRecipe("");
}

Spectrum makeSpectrumWithPlugin(Plugin&& plugin) {
  std::vector<Plugin> plugins;
  plugins.push_back(std::move(plugin));
  return Spectrum(std::move(plugins));
}

Rule makeRule(
    const std::string& ruleName,
    const image::Format& acceptedInputFormat,
    const image::Format& acceptedOutputFormat,
    folly::Optional<Recipe::Factory>&& recipeFactory) {
  return Rule{
      .name = ruleName,
      .allowedInputFormats = {acceptedInputFormat},
      .allowedOutputFormats = {acceptedOutputFormat},
      .recipeFactory =
          recipeFactory.hasValue() ? std::move(*recipeFactory) : nullptr,
  };
}

TranscodeOptions makeDummyTranscodeOptions() {
  return TranscodeOptions(requirements::Encode{.format = image::formats::Jpeg});
}

} // namespace testutils
} // namespace spectrum
} // namespace facebook
