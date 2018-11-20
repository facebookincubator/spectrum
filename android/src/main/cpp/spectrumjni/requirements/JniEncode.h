// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/requirements/Encode.h>

#include <spectrumjni/image/JniFormat.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {
namespace requirements {

class JEncode : public facebook::jni::JavaClass<JEncode> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/requirements/EncodeRequirement;";

  class JMode : public facebook::jni::JavaClass<JMode> {
   public:
    static constexpr const char* kJavaDescriptor =
        "Lcom/facebook/spectrum/requirements/EncodeRequirement$Mode;";

    Encode::Mode toNative() const;
    static facebook::jni::local_ref<JMode> fromNative(const Encode::Mode mode);

   private:
    int value() const;
  };

  Encode toNative() const;
  static facebook::jni::local_ref<JEncode> fromNative(const Encode& encode);

 private:
  Encode::Quality quality() const;
  Encode::Mode mode() const;
  image::EncodedFormat format() const;
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
