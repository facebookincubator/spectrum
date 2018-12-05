// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Specification.h>

#include <spectrumjni/image/JniFormat.h>
#include <spectrumjni/image/JniGeometry.h>
#include <spectrumjni/image/JniMetadata.h>
#include <spectrumjni/image/JniOrientation.h>
#include <spectrumjni/image/JniPixel.h>

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {
namespace image {

class JChromaSamplingMode
    : public facebook::jni::JavaClass<JChromaSamplingMode> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImageChromaSamplingMode;";

  image::ChromaSamplingMode toNative() const;
  static facebook::jni::local_ref<JChromaSamplingMode> fromNative(
      const image::ChromaSamplingMode& chromaSamplingMode);

 private:
  int value() const;
};

class JSpecification : public facebook::jni::JavaClass<JSpecification> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImageSpecification;";

  image::Specification toNative() const;
  static facebook::jni::local_ref<JSpecification> fromNative(
      const image::Specification& specification);

 private:
  Size size() const;
  Format format() const;
  pixel::Specification pixelSpecification() const;
  Orientation orientation() const;
  folly::Optional<ChromaSamplingMode> chromaSamplingMode() const;
  Metadata metadata() const;
};

} // namespace image
} // namespace spectrum
} // namespace facebook
