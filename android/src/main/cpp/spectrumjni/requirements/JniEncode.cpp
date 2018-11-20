// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniEncode.h"

#include <spectrum/core/utils/Numeric.h>

namespace facebook {
namespace spectrum {
namespace requirements {

//
// Mode
//

Encode::Mode JEncode::JMode::toNative() const {
  return Encode::modeFromValue(value());
}

facebook::jni::local_ref<JEncode::JMode> JEncode::JMode::fromNative(
    const Encode::Mode mode) {
  static const auto method =
      javaClassStatic()->getStaticMethod<facebook::jni::local_ref<JMode>(jint)>(
          "from");
  return method(javaClassStatic(), static_cast<int>(mode));
}

int JEncode::JMode::value() const {
  static const auto field = javaClassStatic()->getField<jint>("value");
  return getFieldValue(field);
}

//
// Encode
//

Encode JEncode::toNative() const {
  return Encode{
      .format = format(),
      .mode = mode(),
      .quality = quality(),
  };
}

facebook::jni::local_ref<JEncode> JEncode::fromNative(const Encode& encode) {
  return newInstance(
      image::JEncodedFormat::fromNative(encode.format),
      SPECTRUM_CONVERT_OR_THROW(encode.quality, jint),
      JMode::fromNative(encode.mode));
}

Encode::Quality JEncode::quality() const {
  static const auto field = javaClassStatic()->getField<jint>("quality");
  return SPECTRUM_CONVERT_OR_THROW(getFieldValue(field), Encode::Quality);
}

Encode::Mode JEncode::mode() const {
  static const auto field =
      javaClassStatic()->getField<JMode::javaobject>("mode");
  return getFieldValue(field)->toNative();
}

image::EncodedFormat JEncode::format() const {
  static const auto field =
      javaClassStatic()->getField<image::JEncodedFormat::javaobject>("format");
  return getFieldValue(field)->toNative();
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
