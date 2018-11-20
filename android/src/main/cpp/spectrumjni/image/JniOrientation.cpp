// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniOrientation.h"

#include <fbjni/fbjni.h>
#include <spectrum/core/utils/Numeric.h>

namespace facebook {
namespace spectrum {
namespace image {

Orientation JOrientation::toNative() const {
  return orientationFromValue(
      SPECTRUM_CONVERT_OR_THROW(value(), std::uint16_t));
}

facebook::jni::local_ref<JOrientation> JOrientation::fromNative(
    const Orientation& orientation) {
  static const auto method =
      javaClassStatic()
          ->getStaticMethod<facebook::jni::local_ref<JOrientation>(short)>(
              "from");
  return method(javaClassStatic(), static_cast<short>(orientation));
}

short JOrientation::value() const {
  static const auto field = javaClassStatic()->getField<jshort>("value");
  return getFieldValue(field);
}

} // namespace image
} // namespace spectrum
} // namespace facebook
