// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniImageColor.h"

#include <spectrum/Options.h>

#include <fbjni/fbjni.h>
#include <folly/FixedString.h>

namespace facebook {
namespace spectrum {
namespace image {

image::Color JColor::toNative() const {
  return Color{static_cast<std::uint8_t>(red()),
               static_cast<std::uint8_t>(green()),
               static_cast<std::uint8_t>(blue())};
}

facebook::jni::local_ref<JColor> JColor::fromNative(const image::Color& color) {
  return newInstance(
      static_cast<jint>(color.red),
      static_cast<jint>(color.green),
      static_cast<jint>(color.blue));
}

int JColor::red() const {
  static const auto redField = javaClassStatic()->getField<jint>("red");
  const auto jRed = getFieldValue(redField);
  SPECTRUM_ENFORCE_IF_NOT(jRed >= 0 && jRed <= 255);
  return jRed;
}

int JColor::green() const {
  static const auto greenField = javaClassStatic()->getField<jint>("green");
  const auto jGreen = getFieldValue(greenField);
  SPECTRUM_ENFORCE_IF_NOT(jGreen >= 0 && jGreen <= 255);
  return jGreen;
}

int JColor::blue() const {
  static const auto blueField = javaClassStatic()->getField<jint>("blue");
  const auto jBlue = getFieldValue(blueField);
  SPECTRUM_ENFORCE_IF_NOT(jBlue >= 0 && jBlue <= 255);
  return jBlue;
}

} // namespace image
} // namespace spectrum
} // namespace facebook
