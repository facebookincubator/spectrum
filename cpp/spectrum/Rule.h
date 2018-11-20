// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Recipe.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace facebook {
namespace spectrum {

/**
 * Rules encaplutate the supported actions a recipe can handle.
 */
struct Rule {
 public:
  /**
   * Rotation options supported by rules.
   */
  enum class RotateSupport {
    /** No rotation supported */
    None,

    /** Only rotations multiple of 90 degrees supported */
    MultipleOf90,

    /** Only rotations multiple of 90 degrees and flips supported */
    MultipleOf90Flip,

    /** Only rotations multiple of 180 degrees supported */
    MultipleOf180,
  };

  /**
   * Crop options supported by rules.
   */
  enum class CropSupport {
    /** No cropping supported */
    None,

    /** Only exact cropping supported */
    Exact,

    /** Only approximate cropping supported */
    Approximate,
  };

  /**
   * Resize options supported by rules.
   */
  enum class ResizeSupport {
    /** No resizing supported */
    None,

    /** Only exact resizing supported */
    Exact,

    /** Only exact or larger resizing supported */
    ExactOrLarger,

    /** Only exact or smaller resizing supported */
    ExactOrSmaller,
  };

  /**
   * The name of the rule.
   */
  std::string name;

  /**
   * Factory to create the rule's recipe.
   */
  Recipe::Factory recipeFactory;

  /**
   * Vector of input formats the rule will match against. If empty, no input
   * format restriction.
   */
  std::vector<image::Format> allowedInputFormats;

  /**
   * Vector of output formats the rule will match against. If empty, no output
   * format restriction.
   */
  std::vector<image::Format> allowedOutputFormats;

  /**
   * If true, the rule will only match operations with equal input and output
   * formats.
   */
  bool requiresEqualInputOutputFormat{false};

  /**
   * If true, only operations that allow passthrough rules will be matched. A
   * passthrough rule is a rule that may simply copy the bytes from the source
   * to the destination and not force the encoding / decoding steps when
   * transcoding a file that already fits within the operation's requirements.
   */
  bool isPassthrough{false};

  /**
   * Croppoing support level.
   */
  CropSupport cropSupport{CropSupport::None};

  /**
   * Resizing support level.
   */
  ResizeSupport resizeSupport{ResizeSupport::None};

  /**
   * Rotation support level.
   */
  RotateSupport rotateSupport{RotateSupport::None};
};

} // namespace spectrum
} // namespace facebook
