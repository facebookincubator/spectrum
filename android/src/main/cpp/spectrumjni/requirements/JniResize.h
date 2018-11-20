// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Geometry.h>
#include <spectrum/requirements/Resize.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>
#include <string>

namespace facebook {
namespace spectrum {
namespace requirements {

class JResize : public facebook::jni::JavaClass<JResize> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/requirements/ResizeRequirement;";

  Resize toNative() const;
  static facebook::jni::local_ref<JResize> fromNative(const Resize& resize);

  class JMode : public facebook::jni::JavaClass<JMode> {
   public:
    static constexpr const char* kJavaDescriptor =
        "Lcom/facebook/spectrum/requirements/ResizeRequirement$Mode;";

    Resize::Mode toNative() const;
    static facebook::jni::local_ref<JMode> fromNative(const Resize::Mode mode);

   private:
    int value() const;
  };

 private:
  Resize::Mode mode() const;
  image::Size targetSize() const;
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
