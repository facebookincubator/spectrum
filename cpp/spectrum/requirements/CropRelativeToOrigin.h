// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/requirements/Crop.h>

#include <string>

namespace facebook {
namespace spectrum {
namespace requirements {

/**
 * Crop requirement based on relative offsets to the origin plane (left or top).
 */
struct CropRelativeToOrigin : public Crop {
  /**
   * Relative offsets to crop off the image. Each offset is relative to their
   * axis'length and the top/left edge. For example, {0, 0.5, 1.0, 1.0} would
   * return the top right quarter of the image.
   */
  struct Values {
    float top{0};
    float left{0};
    float bottom{0};
    float right{0};

    /**
     * Returns true if the values are coherent and would return a non-null size.
     */
    bool valid() const;

    /**
     * String representation of values.
     */
    std::string string() const;

    /**
     * Creates a new set of rotated values.
     *
     * @param rotateRequirement The rotate requirement to rotate the values
     * with.
     * @param size The size of the image, before rotation.
     */
    Values rotate(const Rotate& rotateRequirement, const image::Size& size)
        const;
  };

  /**
   * Creates a new crop requirement.
   *
   * @param values The values for the crop requirement.
   * @param mustBeExact Wether the crop region has to match exactly.
   */
  CropRelativeToOrigin(const Values& values, const bool mustBeExact);

  /**
   * The relative values to use for cropping.
   */
  Values values() const;

 private:
  class Handler : public IHandler {
   public:
    Handler(const CropRelativeToOrigin::Values& values);

    image::Rect apply(const image::Size& size) const override;
    std::unique_ptr<IHandler> rotated(
        const Rotate& rotateRequirement,
        const image::Size& size) const override;
    std::unique_ptr<IHandler> scaled(const image::Ratio& ratio) const override;
    std::unique_ptr<IHandler> copy() const override;
    std::string string() const override;

    const Values values;
  };
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
