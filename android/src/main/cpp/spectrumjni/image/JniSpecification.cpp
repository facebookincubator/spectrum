// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpecification.h"

#include <fbjni/fbjni.h>
#include <spectrumjni/JniUtils.h>

namespace facebook {
namespace spectrum {
namespace image {

//
// JChromaSamplingMode
//

image::ChromaSamplingMode JChromaSamplingMode::toNative() const {
  return image::chromaSamplingModeFromValue(value());
}

facebook::jni::local_ref<JChromaSamplingMode> JChromaSamplingMode::fromNative(
    const image::ChromaSamplingMode& chromaSamplingMode) {
  static const auto method =
      javaClassStatic()
          ->getStaticMethod<facebook::jni::local_ref<JChromaSamplingMode>(
              jint)>("from");

  return method(javaClassStatic(), static_cast<jint>(chromaSamplingMode));
}

int JChromaSamplingMode::value() const {
  static const auto field = javaClassStatic()->getField<jint>("value");
  return getFieldValue(field);
}

//
// JSpecification
//

image::Specification JSpecification::toNative() const {
  return image::Specification{size(),
                              format(),
                              pixelSpecification(),
                              orientation(),
                              chromaSamplingMode(),
                              metadata()};
}

facebook::jni::local_ref<JSpecification> JSpecification::fromNative(
    const image::Specification& specification) {
  return newInstance(
      JSize::fromNative(specification.size),
      JFormat::fromNative(specification.format),
      pixel::JSpecification::fromNative(specification.pixelSpecification),
      JOrientation::fromNative(specification.orientation),
      jni::optionalToNullableJavaObject<
          decltype(specification.chromaSamplingMode),
          JChromaSamplingMode>(specification.chromaSamplingMode),
      JMetadata::fromNative(specification.metadata));
}

Size JSpecification::size() const {
  static const auto field =
      javaClassStatic()->getField<JSize::javaobject>("size");
  return getFieldValue(field)->toNative();
}

Format JSpecification::format() const {
  static const auto field =
      javaClassStatic()->getField<JFormat::javaobject>("format");
  return getFieldValue(field)->toNative();
}

pixel::Specification JSpecification::pixelSpecification() const {
  static const auto field =
      javaClassStatic()->getField<pixel::JSpecification::javaobject>(
          "pixelSpecification");
  return getFieldValue(field)->toNative();
}

Orientation JSpecification::orientation() const {
  static const auto field =
      javaClassStatic()->getField<JOrientation::javaobject>("orientation");
  return getFieldValue(field)->toNative();
}

folly::Optional<ChromaSamplingMode> JSpecification::chromaSamplingMode() const {
  static const auto field =
      javaClassStatic()->getField<JChromaSamplingMode::javaobject>(
          "chromaSamplingMode");
  return jni::nullableJavaObjectToOptional<ChromaSamplingMode>(
      getFieldValue(field));
}

Metadata JSpecification::metadata() const {
  static const auto field =
      javaClassStatic()->getField<JMetadata::javaobject>("metadata");
  return getFieldValue(field)->toNative();
}

} // namespace image
} // namespace spectrum
} // namespace facebook
