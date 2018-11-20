// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Orientation.h>

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {
namespace image {

class JOrientation : public facebook::jni::JavaClass<JOrientation> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImageOrientation;";

  Orientation toNative() const;
  static facebook::jni::local_ref<JOrientation> fromNative(
      const Orientation& orientation);

 private:
  short value() const;
};
} // namespace image
} // namespace spectrum
} // namespace facebook
