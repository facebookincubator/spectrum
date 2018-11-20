// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Geometry.h>
#include <spectrum/requirements/Crop.h>

#include <string>

namespace facebook {
namespace spectrum {
namespace requirements {

/**
 * Crop requirement based on absolute offsets to the origin plane (left or top).
 * Do not add to this class as it will get sliced.
 */
struct CropAbsoluteToOrigin : public Crop {
  /**
   * Absolute offsets to crop off the image. Each offset is relative to the
   * top/left edge. For example, {0, 0, 1, 1} would return the first pixel on
   * the top left.
   */
  struct Values {
    std::uint32_t top{0};
    std::uint32_t left{0};
    std::uint32_t bottom{0};
    std::uint32_t right{0};

    /**
     * Returns true if the values are coherent and would return a non-null size.
     */
    bool valid() const;

    /**
     * Returns true if the values are contained within size.
     */
    bool validWithSize(const image::Size& size) const;

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
  CropAbsoluteToOrigin(const Values& values, const bool mustBeExact);

  /**
   * The absolute values to use for cropping.
   */
  Values values() const;

 private:
  class Handler : public Crop::IHandler {
   public:
    Handler(const Values& values);

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
