// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/Operation.h>
#include <spectrum/image/Specification.h>

#include <memory>

namespace facebook {
namespace spectrum {

/**
 * Recipes contain the core of the image processing logic. Created by rules,
 * they are responsible for performing the provided operation.
 */
class Recipe {
 public:
  using Factory = std::function<std::unique_ptr<Recipe>()>;

  virtual ~Recipe() = default;

  /**
   * Performs the operation from source into a new image that is written into
   * sink based on the requirements in Options. Returns the ImageInformation of
   * the newly created image.
   */
  virtual image::Specification perform(
      const core::Operation& operation) const = 0;
};

} // namespace spectrum
} // namespace facebook
