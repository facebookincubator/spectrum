// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Pixel.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <sstream>
#include <string>

namespace facebook {
namespace spectrum {
namespace image {
namespace pixel {

//
// ColorModel
//

bool ColorModel::operator==(const ColorModel& rhs) const {
  return identifier == rhs.identifier &&
      numberOfColorComponents == rhs.numberOfColorComponents &&
      supportsExtraAlphaChannel == rhs.supportsExtraAlphaChannel;
}

bool ColorModel::operator<(const ColorModel& rhs) const {
  return identifier < rhs.identifier &&
      numberOfColorComponents < rhs.numberOfColorComponents &&
      supportsExtraAlphaChannel < rhs.supportsExtraAlphaChannel;
}

//
// AlphaInfo
//

std::string alphaInfoValueToString(const AlphaInfo alphaInfo) {
  switch (alphaInfo) {
    case AlphaInfo::None:
      return "none";
    case AlphaInfo::First:
      return "first";
    case AlphaInfo::Last:
      return "last";
    case AlphaInfo::SkipFirst:
      return "skipFirst";
    case AlphaInfo::SkipLast:
      return "skipLast";
    case AlphaInfo::PremultipliedFirst:
      return "premultipliedFirst";
    case AlphaInfo::PremultipliedLast:
      return "premultipliedLast";
  }
}

AlphaInfo alphaInfoFromValue(const int value) {
  SPECTRUM_ENFORCE_IF_NOT(
      value >= static_cast<int>(AlphaInfo::None) &&
      value <= static_cast<int>(AlphaInfo::PremultipliedLast));
  return static_cast<AlphaInfo>(value);
}

bool alphaInfoIsFirst(const AlphaInfo alphaInfo) {
  switch (alphaInfo) {
    case AlphaInfo::First:
    case AlphaInfo::SkipFirst:
    case AlphaInfo::PremultipliedFirst:
      return true;
    case AlphaInfo::None:
    case AlphaInfo::Last:
    case AlphaInfo::SkipLast:
    case AlphaInfo::PremultipliedLast:
      return false;
  }
}

bool alphaInfoIsLast(const AlphaInfo alphaInfo) {
  switch (alphaInfo) {
    case AlphaInfo::None:
    case AlphaInfo::First:
    case AlphaInfo::SkipFirst:
    case AlphaInfo::PremultipliedFirst:
      return false;
    case AlphaInfo::Last:
    case AlphaInfo::SkipLast:
    case AlphaInfo::PremultipliedLast:
      return true;
  }
}

bool alphaInfoIsPremultiplied(const AlphaInfo alphaInfo) {
  switch (alphaInfo) {
    case AlphaInfo::PremultipliedFirst:
    case AlphaInfo::PremultipliedLast:
      return true;
    case AlphaInfo::None:
    case AlphaInfo::SkipFirst:
    case AlphaInfo::SkipLast:
    case AlphaInfo::First:
    case AlphaInfo::Last:
      return false;
  }
}

bool alphaInfoShouldInterpretAlphaValue(const AlphaInfo alphaInfo) {
  switch (alphaInfo) {
    case AlphaInfo::First:
    case AlphaInfo::Last:
    case AlphaInfo::PremultipliedFirst:
    case AlphaInfo::PremultipliedLast:
      return true;
    case AlphaInfo::None:
    case AlphaInfo::SkipFirst:
    case AlphaInfo::SkipLast:
      return false;
  }
}

//
// ComponentsOrder
//

std::string componentsOrderValueToString(
    const ComponentsOrder componentsOrder) {
  switch (componentsOrder) {
    case ComponentsOrder::Natural:
      return "natural";
    case ComponentsOrder::Reversed:
      return "reversed";
  }
}

ComponentsOrder makeComponentsOrderFromValue(const int value) {
  SPECTRUM_ENFORCE_IF_NOT(
      value >= static_cast<int>(ComponentsOrder::Natural) &&
      value <= static_cast<int>(ComponentsOrder::Reversed));
  return static_cast<ComponentsOrder>(value);
}

//
// Specification
//

std::uint8_t Specification::numberOfComponents() const noexcept {
  const std::uint8_t numberOfColorComponents =
      colorModel.numberOfColorComponents;
  return hasAlpha() ? numberOfColorComponents + 1 : numberOfColorComponents;
}

bool Specification::hasAlpha() const noexcept {
  return colorModel.supportsExtraAlphaChannel && alphaInfo != AlphaInfo::None;
}

bool Specification::isAlphaLeadingComponent() const noexcept {
  return (
      (alphaInfoIsFirst(alphaInfo) &&
       componentsOrder == ComponentsOrder::Natural) ||
      (alphaInfoIsLast(alphaInfo) &&
       componentsOrder == ComponentsOrder::Reversed));
}

std::string Specification::string() const {
  std::stringstream stringStream;
  stringStream << "colorModel: " << colorModel.identifier;
  stringStream << ", bytesPerPixel: " << static_cast<int>(bytesPerPixel);
  stringStream << ", alphaInfo: " << alphaInfoValueToString(alphaInfo);
  stringStream << ", componentsOrder: "
               << componentsOrderValueToString(componentsOrder);
  return stringStream.str();
}

bool Specification::operator==(const Specification& rhs) const {
  return std::tie(colorModel, bytesPerPixel, alphaInfo, componentsOrder) ==
      std::tie(
             rhs.colorModel,
             rhs.bytesPerPixel,
             rhs.alphaInfo,
             rhs.componentsOrder);
}

bool Specification::operator!=(const Specification& rhs) const {
  return !(*this == rhs);
}

bool Specification::operator<(const Specification& rhs) const noexcept {
  return colorModel < rhs.colorModel && bytesPerPixel < rhs.bytesPerPixel &&
      alphaInfo < rhs.alphaInfo && componentsOrder < rhs.componentsOrder;
}

} // namespace pixel
} // namespace image
} // namespace spectrum
} // namespace facebook
