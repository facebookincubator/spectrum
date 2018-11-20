// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Geometry.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>
#include <string>

namespace facebook {
namespace spectrum {
namespace image {

class JSize : public facebook::jni::JavaClass<JSize> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImageSize;";

  image::Size toNative() const;
  static facebook::jni::local_ref<JSize> fromNative(const image::Size& size);

 private:
  int width() const;
  int height() const;
};

} // namespace image
} // namespace spectrum
} // namespace facebook
