// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniRotate.h"

namespace facebook {
namespace spectrum {
namespace requirements {

Rotate JRotate::toNative() const {
  return Rotate{
      .degrees = degrees(),
      .flipHorizontally = flipHorizontally(),
      .flipVertically = flipVertically(),
      .forceUpOrientation = forceUpOrientation(),
  };
}

facebook::jni::local_ref<JRotate> JRotate::fromNative(const Rotate& rotate) {
  return newInstance(
      rotate.degrees,
      rotate.flipHorizontally,
      rotate.flipVertically,
      rotate.forceUpOrientation);
}

int JRotate::degrees() const {
  static const auto field = javaClassStatic()->getField<jint>("degrees");
  return getFieldValue(field);
}

bool JRotate::flipHorizontally() const {
  static const auto field =
      javaClassStatic()->getField<jboolean>("flipHorizontally");
  return getFieldValue(field);
}

bool JRotate::flipVertically() const {
  static const auto field =
      javaClassStatic()->getField<jboolean>("flipVertically");
  return getFieldValue(field);
}

bool JRotate::forceUpOrientation() const {
  static const auto field =
      javaClassStatic()->getField<jboolean>("forceUpOrientation");
  return getFieldValue(field);
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
