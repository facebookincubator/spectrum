// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniResize.h"

#include <spectrumjni/image/JniGeometry.h>

namespace facebook {
namespace spectrum {
namespace requirements {

//
// Resize
//

Resize JResize::toNative() const {
  return Resize{
      .mode = mode(),
      .targetSize = targetSize(),
  };
}

facebook::jni::local_ref<JResize> JResize::fromNative(const Resize& resize) {
  return newInstance(
      JResize::JMode::fromNative(resize.mode),
      image::JSize::fromNative(resize.targetSize));
}

Resize::Mode JResize::mode() const {
  static const auto field =
      javaClassStatic()->getField<JResize::JMode::javaobject>("mode");
  return getFieldValue(field)->toNative();
}

image::Size JResize::targetSize() const {
  static const auto field =
      javaClassStatic()->getField<image::JSize::javaobject>("targetSize");
  return getFieldValue(field)->toNative();
}

//
// Mode
//

Resize::Mode JResize::JMode::toNative() const {
  return requirements::Resize::makeModeFromValue(value());
}

facebook::jni::local_ref<JResize::JMode> JResize::JMode::fromNative(
    const Resize::Mode mode) {
  static const auto method =
      javaClassStatic()
          ->getStaticMethod<facebook::jni::local_ref<JResize::JMode>(jint)>(
              "from");
  return method(javaClassStatic(), static_cast<std::uint32_t>(mode));
}

int JResize::JMode::value() const {
  static const auto field = javaClassStatic()->getField<jint>("value");
  return getFieldValue(field);
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
