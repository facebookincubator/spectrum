// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once
#include <fbjni/fbjni.h>
#include <spectrum/Configuration.h>

namespace facebook {
namespace spectrum {
namespace image {

class JImageHint : public facebook::jni::JavaClass<JImageHint> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/Configuration$ImageHint;";

  Configuration::Webp::ImageHint toNative() const;
  static facebook::jni::local_ref<JImageHint> fromNative(
      const Configuration::Webp::ImageHint& webpImageHint);

 private:
  int value() const;
};

} // namespace image
} // namespace spectrum
} // namespace facebook
