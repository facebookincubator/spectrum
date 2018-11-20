// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Recipe.h>
#include <spectrum/Rule.h>

namespace facebook {
namespace spectrum {
namespace core {
namespace recipes {

class CopyRecipe : public Recipe {
 public:
  image::Specification perform(const core::Operation& operation) const override;

  static Rule makeRule();
};

} // namespace recipes
} // namespace core
} // namespace spectrum
} // namespace facebook
