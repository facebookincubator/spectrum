// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Format.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>
#include <string>

namespace facebook {
namespace spectrum {
namespace image {

class JFormat : public facebook::jni::JavaClass<JFormat> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImageFormat;";

  image::Format toNative() const;
  static facebook::jni::local_ref<JFormat> fromNative(
      const image::Format& format);

 protected:
  std::string identifier() const;
  bool encoded() const;
};

class JEncodedFormat
    : public facebook::jni::JavaClass<JEncodedFormat, JFormat> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/EncodedImageFormat;";

  image::EncodedFormat toNative() const;
  static facebook::jni::local_ref<JEncodedFormat> fromNative(
      const image::Format& format);
};

} // namespace image
} // namespace spectrum
} // namespace facebook
