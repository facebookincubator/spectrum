// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/requirements/Crop.h>

namespace facebook {
namespace spectrum {
namespace requirements {

/**
 * Crop requirement that returns the intersection of the rect with the image
 * size. Do not add to this class as it will get sliced.
 */
struct CropRect : public Crop {
  /**
   * Creates a new crop requirement.
   *
   * @param rect The rect to crop.
   * @param mustBeExact Wether the crop region has to match exactly.
   */
  CropRect(const image::Rect& rect, const bool mustBeExact);

  /**
   * The rect to use for cropping.
   */
  image::Rect rect() const;

 private:
  class Handler : public IHandler {
   public:
    Handler(const image::Rect& rect);

    image::Rect apply(const image::Size& size) const override;
    std::unique_ptr<IHandler> rotated(
        const Rotate& rotateRequirement,
        const image::Size& size) const override;
    std::unique_ptr<IHandler> scaled(const image::Ratio& ratio) const override;
    std::unique_ptr<IHandler> copy() const override;
    std::string string() const override;

    const image::Rect rect;
  };
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
