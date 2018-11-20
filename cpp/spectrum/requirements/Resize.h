// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Geometry.h>

namespace facebook {
namespace spectrum {
namespace requirements {

/**
 * Defines how an image should be resized.
 *
 * @note A resize requirement will never affect the aspect ratio of an image and
 * will never upscale an image.
 */
struct Resize {
  /**
   * Defines the strictness of the resize requirement in accepting values other
   * than the specified target size.
   */
  enum class Mode : std::uint32_t {

    /**
     * When resizing, the output image will have the exact target size.
     */
    Exact = 0,

    /**
     * When resizing, the output image might be slightly smaller than the target
     * size. This can allow for more efficient operations.
     */
    ExactOrSmaller = 1,

    /**
     * When resizing, the output image might be slightly larger than the target
     * size. This can allow for more efficient operations.
     */
    ExactOrLarger = 2,
  };

  static Mode makeModeFromValue(const std::uint32_t value);

  /**
   * The mode to resize the image with.
   */
  Mode mode{Mode::Exact};

  /**
   * The target size to resize the image to.
   */
  image::Size targetSize{image::sizeZero};

  bool operator==(const Resize& rhs) const;
  bool operator!=(const Resize& rhs) const;
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
