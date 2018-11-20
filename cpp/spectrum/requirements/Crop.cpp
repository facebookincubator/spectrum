// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Crop.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <sstream>

namespace facebook {
namespace spectrum {
namespace requirements {
namespace error {
const folly::StringPiece CropCannotRotateWithRequirement{
    "crop_requirement_cannot_rotate_with_requirement"};
}

Crop::Crop(std::unique_ptr<IHandler> handler, const bool mustBeExact)
    : mustBeExact(mustBeExact),
      _handler(std::move(handler)),
      _handlerStringRepresentation(_handler->string()) {
  SPECTRUM_ENFORCE_IF(_handler == nullptr);
}

Crop::Crop(const Crop& other)
    : mustBeExact(other.mustBeExact),
      _handler(other._handler->copy()),
      _handlerStringRepresentation(other._handlerStringRepresentation) {}

Crop& Crop::operator=(const Crop& other) {
  mustBeExact = other.mustBeExact;
  _handler = other._handler->copy();
  _handlerStringRepresentation = other._handlerStringRepresentation;
  return *this;
}

bool Crop::operator==(const Crop& rhs) const {
  // We rely on the string representation of the handlers to compare them.
  // Yuck, IKR - but it's the best compromise I can think of for now.
  return mustBeExact == rhs.mustBeExact &&
      _handlerStringRepresentation == rhs._handlerStringRepresentation;
}

image::Rect Crop::apply(const image::Size& size) const {
  return _handler->apply(size);
}

Crop Crop::rotated(const Rotate& rotateRequirement, const image::Size& size)
    const {
  SPECTRUM_ERROR_STRING_IF(
      rotateRequirement.flip() || rotateRequirement.forceUpOrientation ||
          !core::numeric::isMultipleOf90(rotateRequirement.degrees),
      error::CropCannotRotateWithRequirement,
      rotateRequirement.string());

  return Crop(_handler->rotated(rotateRequirement, size), mustBeExact);
}

Crop Crop::scaled(const image::Ratio& ratio) const {
  SPECTRUM_ENFORCE_IF(ratio.zero());

  if (ratio.one()) {
    return *this;
  } else {
    return Crop(_handler->scaled(ratio), mustBeExact);
  }
}

std::string Crop::string() const {
  std::stringstream ss;
  ss << "{mustBeExact:" << mustBeExact;
  ss << ",";
  ss << "handler:" << _handlerStringRepresentation << "}";
  return ss.str();
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
