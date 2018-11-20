// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Geometry.h>
#include <spectrum/requirements/Rotate.h>

#include <folly/Range.h>

#include <memory>
#include <string>

namespace facebook {
namespace spectrum {
namespace requirements {
namespace error {
extern const folly::StringPiece CropCannotRotateWithRequirement;
}

/**
 * Defines how an image should be cropped.
 */
struct Crop {
  /**
   * True if the crop has to be exact. Non exact crop can be more efficient.
   */
  bool mustBeExact{true};

  /**
   * Copy constructor.
   *
   * @param other The crop requirement to copy.
   */
  Crop(const Crop& other);

  /**
   * Move constructor.
   *
   * @param other The crop requirement to move.
   */
  Crop(Crop&& other) = default;

  /**
   * The final rect of interest after the cropping operation.
   *
   * @param size The size of the image to crop.
   */
  image::Rect apply(const image::Size& size) const;

  /**
   * Creates a new rotated crop requirement.
   *
   * @param rotateRequirement The rotate requirement to apply.
   * @param size The size of the image.
   */
  Crop rotated(const Rotate& rotateRequirement, const image::Size& size) const;

  /**
   * Creates a new scaled crop requirement.
   *
   * @param ratio The ratio to scale the crop requirement.
   */
  Crop scaled(const image::Ratio& ratio) const;

  /**
   * String representation of a crop requirement.
   */
  std::string string() const;

  Crop& operator=(const Crop& other);
  Crop& operator=(Crop&& other) = default;
  bool operator==(const Crop& rhs) const;

 protected:
  /**
   * Handler performing the actual crop logic. Subclasses need to provide a
   * polymorphic instance of IHandler.
   */
  class IHandler {
   public:
    virtual ~IHandler() {}

    /**
     * The final rect of interest after the cropping operation.
     *
     * @param size The size of the image to crop.
     */
    virtual image::Rect apply(const image::Size& size) const = 0;

    /**
     * Creates a new rotated handler.
     *
     * @param rotateRequirement The rotate requirement to apply.
     * @param size The size of the image.
     */
    virtual std::unique_ptr<IHandler> rotated(
        const Rotate& rotateRequirement,
        const image::Size& size) const = 0;

    /**
     * Creates a new scaled handler.
     *
     * @param ratio The ratio to scale the handler with.
     */
    virtual std::unique_ptr<IHandler> scaled(
        const image::Ratio& ratio) const = 0;

    /**
     * Returns a new equal handler.
     */
    virtual std::unique_ptr<IHandler> copy() const = 0;

    /**
     * Returns the string representation of the handler. This must include all
     * values as it gets used for equality.
     */
    virtual std::string string() const = 0;
  };

  /**
   * Creates a new cropping requirement.
   *
   * @param handler The handler to use.
   * @param mustBeExact Wether the crop region has to match exactly.
   */
  Crop(std::unique_ptr<IHandler> handler, const bool mustBeExact);

  /**
   * The crop requirement's handler.
   */
  std::unique_ptr<IHandler> _handler;

 private:
  /**
   * String representation of the handler cached for efficiency.
   */
  std::string _handlerStringRepresentation;
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
