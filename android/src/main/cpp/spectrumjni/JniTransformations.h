// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Transformations.h>

#include <spectrumjni/image/JniFormat.h>
#include <spectrumjni/requirements/JniCropRequirement.h>
#include <spectrumjni/requirements/JniResize.h>
#include <spectrumjni/requirements/JniRotate.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {

class JTransformations : public facebook::jni::JavaClass<JTransformations> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/options/Transformations;";

  Transformations toNative() const;
  static facebook::jni::local_ref<JTransformations> fromNative(
      const Transformations& transformations);

 private:
  folly::Optional<requirements::Resize> resizeRequirement() const;
  folly::Optional<requirements::Rotate> rotateRequirement() const;
  folly::Optional<requirements::Crop> cropRequirement() const;
};

} // namespace spectrum
} // namespace facebook
