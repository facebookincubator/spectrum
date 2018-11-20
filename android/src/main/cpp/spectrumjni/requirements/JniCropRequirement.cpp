// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniCropRequirement.h"

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {
namespace requirements {

//
// JCropRequirement
//

Crop JCropRequirement::toNative() const {
  // static_ref_cast only accepts "plain references", thus requiring this cast
  facebook::jni::alias_ref<jobject> plainSelfReference = self();

  if (isInstanceOf(JAbsoluteCropRequirement::javaClassStatic())) {
    return facebook::jni::static_ref_cast<JAbsoluteCropRequirement::javaobject>(
               plainSelfReference)
        ->toNative();

  } else if (isInstanceOf(JRelativeCropRequirement::javaClassStatic())) {
    return facebook::jni::static_ref_cast<JRelativeCropRequirement::javaobject>(
               plainSelfReference)
        ->toNative();

  } else {
    SPECTRUM_ERROR("unsupported_crop_requirement");
  }
}

std::string JCropRequirement::toString(const Crop& crop) {
  return crop.string();
}

bool JCropRequirement::mustBeExact() const {
  static const auto field =
      javaClassStatic()->getField<jboolean>("mustBeExact");
  return getFieldValue(field);
}

//
// JAbsoluteCropRequirement
//

Crop JAbsoluteCropRequirement::toNative() const {
  return CropAbsoluteToOrigin(
      CropAbsoluteToOrigin::Values{
          SPECTRUM_CONVERT_OR_THROW(top(), std::uint32_t),
          SPECTRUM_CONVERT_OR_THROW(left(), std::uint32_t),
          SPECTRUM_CONVERT_OR_THROW(bottom(), std::uint32_t),
          SPECTRUM_CONVERT_OR_THROW(right(), std::uint32_t)},
      mustBeExact());
}

int JAbsoluteCropRequirement::top() const {
  static const auto field = javaClassStatic()->getField<int>("top");
  return getFieldValue(field);
}

int JAbsoluteCropRequirement::left() const {
  static const auto field = javaClassStatic()->getField<int>("left");
  return getFieldValue(field);
}

int JAbsoluteCropRequirement::bottom() const {
  static const auto field = javaClassStatic()->getField<int>("bottom");
  return getFieldValue(field);
}

int JAbsoluteCropRequirement::right() const {
  static const auto field = javaClassStatic()->getField<int>("right");
  return getFieldValue(field);
}

//
// JRelativeCropRequirement
//

Crop JRelativeCropRequirement::toNative() const {
  return CropRelativeToOrigin(
      CropRelativeToOrigin::Values{top(), left(), bottom(), right()},
      mustBeExact());
}

float JRelativeCropRequirement::top() const {
  static const auto field = javaClassStatic()->getField<float>("top");
  return getFieldValue(field);
}

float JRelativeCropRequirement::left() const {
  static const auto field = javaClassStatic()->getField<float>("left");
  return getFieldValue(field);
}

float JRelativeCropRequirement::bottom() const {
  static const auto field = javaClassStatic()->getField<float>("bottom");
  return getFieldValue(field);
}

float JRelativeCropRequirement::right() const {
  static const auto field = javaClassStatic()->getField<float>("right");
  return getFieldValue(field);
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
