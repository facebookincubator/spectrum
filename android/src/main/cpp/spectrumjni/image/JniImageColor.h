// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Color.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>
#include <string>

namespace facebook {
namespace spectrum {
namespace image {

class JColor : public facebook::jni::JavaClass<JColor> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImageColor;";

  image::Color toNative() const;
  static facebook::jni::local_ref<JColor> fromNative(const image::Color& color);

 private:
  int red() const;
  int green() const;
  int blue() const;
};

} // namespace image
} // namespace spectrum
} // namespace facebook