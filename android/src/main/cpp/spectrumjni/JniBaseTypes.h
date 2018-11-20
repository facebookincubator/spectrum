// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/requirements/Crop.h>
#include <spectrum/requirements/Resize.h>
#include <spectrumjni/image/JniFormat.h>
#include <spectrumjni/image/JniGeometry.h>
#include <spectrumjni/image/JniImageColor.h>
#include <spectrumjni/image/JniOrientation.h>
#include <spectrumjni/image/JniPixel.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {
namespace jni {

class JBoolean : public facebook::jni::JavaClass<JBoolean> {
 public:
  static constexpr auto kJavaDescriptor = "Ljava/lang/Boolean;";
  bool toNative() const;
  static facebook::jni::local_ref<JBoolean> fromNative(const bool value);
};

class JInteger : public facebook::jni::JavaClass<JInteger> {
 public:
  static constexpr auto kJavaDescriptor = "Ljava/lang/Integer;";
  int toNative() const;
  static facebook::jni::local_ref<JInteger> fromNative(const int value);
};

class JBitmap : public facebook::jni::JavaClass<JBitmap> {
 public:
  static constexpr const char* kJavaDescriptor = "Landroid/graphics/Bitmap;";
};

} // namespace jni
} // namespace spectrum
} // namespace facebook
