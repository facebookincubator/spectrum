// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniTransformations.h"

#include <spectrum/core/utils/Numeric.h>
#include <spectrumjni/JniBaseTypes.h>
#include <spectrumjni/JniUtils.h>
#include <spectrumjni/requirements/JniCropRequirement.h>
#include <spectrumjni/requirements/JniResize.h>

namespace facebook {
namespace spectrum {

Transformations JTransformations::toNative() const {
  return Transformations{
      .resizeRequirement = resizeRequirement(),
      .cropRequirement = cropRequirement(),
      .rotateRequirement = rotateRequirement(),
  };
}

facebook::jni::local_ref<JTransformations> JTransformations::fromNative(
    const Transformations& transformations) {
  // The crop requirement does not have a full bijective transformation
  // (i.e. we cannot create the corresponding Java object for a native
  // value). Therefore, we default to null
  facebook::jni::local_ref<requirements::JCropRequirement> cropRequirement =
      nullptr;

  return newInstance(
      jni::optionalToNullableJavaObject<
          decltype(transformations.resizeRequirement),
          requirements::JResize>(transformations.resizeRequirement),
      cropRequirement,
      jni::optionalToNullableJavaObject<
          decltype(transformations.rotateRequirement),
          requirements::JRotate>(transformations.rotateRequirement));
}

folly::Optional<requirements::Resize> JTransformations::resizeRequirement()
    const {
  static const auto field =
      javaClassStatic()->getField<requirements::JResize::javaobject>(
          "resizeRequirement");
  const auto jResizeRequirement = getFieldValue(field);

  return jni::nullableJavaObjectToOptional<requirements::Resize>(
      jResizeRequirement);
}

folly::Optional<requirements::Crop> JTransformations::cropRequirement() const {
  static const auto field =
      javaClassStatic()->getField<requirements::JCropRequirement::javaobject>(
          "cropRequirement");
  const auto jCropRequirement = getFieldValue(field);
  return jni::nullableJavaObjectToOptional<requirements::Crop>(
      jCropRequirement);
}

folly::Optional<requirements::Rotate> JTransformations::rotateRequirement()
    const {
  static const auto field =
      javaClassStatic()->getField<requirements::JRotate::javaobject>(
          "rotateRequirement");
  const auto jRotateRequirement = getFieldValue(field);
  return jni::nullableJavaObjectToOptional<requirements::Rotate>(
      jRotateRequirement);
}

} // namespace spectrum
} // namespace facebook
