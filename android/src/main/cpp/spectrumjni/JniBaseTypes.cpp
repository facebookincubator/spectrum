// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniBaseTypes.h"

#include <spectrum/Options.h>

#include <spectrumjni/image/JniFormat.h>
#include <spectrumjni/image/JniGeometry.h>
#include <spectrumjni/image/JniImageColor.h>

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {
namespace jni {

//
// JBoolean
//

bool JBoolean::toNative() const {
  static const auto method =
      javaClassStatic()->getMethod<jboolean()>("booleanValue");
  return method(self());
}

facebook::jni::local_ref<JBoolean> JBoolean::fromNative(const bool value) {
  return newInstance(value);
}

//
// JInteger
//

int JInteger::toNative() const {
  static const auto method = javaClassStatic()->getMethod<jint()>("intValue");
  return method(self());
}

facebook::jni::local_ref<JInteger> JInteger::fromNative(const int value) {
  return newInstance(value);
}

} // namespace jni
} // namespace spectrum
} // namespace facebook
