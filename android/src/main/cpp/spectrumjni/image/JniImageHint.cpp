// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniImageHint.h"
#include <spectrum/Configuration.h>

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {
namespace image {

//
// JImageHint
//

Configuration::Webp::ImageHint JImageHint::toNative() const {
  return Configuration::Webp::makeImageHintFromValue(JImageHint::value());
}

facebook::jni::local_ref<JImageHint> JImageHint::fromNative(
    const Configuration::Webp::ImageHint& webpImageHint) {
  static const auto webpMethod =
      javaClassStatic()
          ->getStaticMethod<facebook::jni::local_ref<JImageHint>(jint)>("from");
  return webpMethod(javaClassStatic(), static_cast<int>(webpImageHint));
}

int JImageHint::value() const {
  static const auto field = javaClassStatic()->getField<jint>("value");
  return getFieldValue(field);
}

} // namespace image
} // namespace spectrum
} // namespace facebook
