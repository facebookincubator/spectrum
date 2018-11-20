// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniMetadata.h"

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {
namespace image {

image::Metadata JMetadata::toNative() const {
  return image::Metadata();
}

facebook::jni::local_ref<JMetadata> JMetadata::fromNative(
    const image::Metadata& metadata) {
  return JMetadata::newInstance();
}

} // namespace image
} // namespace spectrum
} // namespace facebook
