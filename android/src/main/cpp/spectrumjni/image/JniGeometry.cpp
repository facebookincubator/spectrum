// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniGeometry.h"

#include <spectrum/Options.h>

#include <fbjni/fbjni.h>
#include <folly/FixedString.h>

namespace facebook {
namespace spectrum {
namespace image {

//
// JSize
//

image::Size JSize::toNative() const {
  return Size{
      .width = SPECTRUM_CONVERT_OR_THROW(width(), std::uint32_t),
      .height = SPECTRUM_CONVERT_OR_THROW(height(), std::uint32_t),
  };
}

facebook::jni::local_ref<JSize> JSize::fromNative(const image::Size& size) {
  return newInstance(
      static_cast<jint>(size.width), static_cast<jint>(size.height));
}

int JSize::width() const {
  static const auto field = javaClassStatic()->getField<jint>("width");
  return getFieldValue(field);
}

int JSize::height() const {
  static const auto field = javaClassStatic()->getField<jint>("height");
  return getFieldValue(field);
}

} // namespace image
} // namespace spectrum
} // namespace facebook
