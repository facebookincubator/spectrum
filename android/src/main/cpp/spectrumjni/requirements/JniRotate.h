// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Geometry.h>
#include <spectrum/requirements/Rotate.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>
#include <string>

namespace facebook {
namespace spectrum {
namespace requirements {

class JRotate : public facebook::jni::JavaClass<JRotate> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/requirements/RotateRequirement;";

  Rotate toNative() const;
  static facebook::jni::local_ref<JRotate> fromNative(const Rotate& rotate);

 private:
  int degrees() const;
  bool flipHorizontally() const;
  bool flipVertically() const;
  bool forceUpOrientation() const;
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
