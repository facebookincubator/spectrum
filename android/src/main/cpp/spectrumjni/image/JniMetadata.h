// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Metadata.h>

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {
namespace image {

class JMetadata : public facebook::jni::JavaClass<JMetadata> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImageMetadata;";

  image::Metadata toNative() const;
  static facebook::jni::local_ref<JMetadata> fromNative(
      const image::Metadata& metadata);
};

} // namespace image
} // namespace spectrum
} // namespace facebook
