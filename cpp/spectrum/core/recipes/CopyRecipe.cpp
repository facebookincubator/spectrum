// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "CopyRecipe.h"

#include <spectrum/core/Constants.h>

#include <array>

namespace facebook {
namespace spectrum {
namespace core {
namespace recipes {

image::Specification CopyRecipe::perform(
    const core::Operation& operation) const {
  std::array<char, core::DefaultBufferSize> buffer;

  std::size_t numReadBytes = 0;
  do {
    numReadBytes = operation.io.source.read(buffer.data(), buffer.size());
    operation.io.sink.write(buffer.data(), numReadBytes);
  } while (numReadBytes > 0);

  return operation.parameters.inputImageSpecification;
}

Rule CopyRecipe::makeRule() {
  return {
      .name = "copy",
      .requiresEqualInputOutputFormat = true,
      .isPassthrough = true,
      .cropSupport = Rule::CropSupport::None,
      .resizeSupport = Rule::ResizeSupport::None,
      .rotateSupport = Rule::RotateSupport::None,
      .recipeFactory = &std::make_unique<CopyRecipe>,
  };
}

} // namespace recipes
} // namespace core
} // namespace spectrum
} // namespace facebook
