// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/IBitmapImageSink.h>
#include <spectrumjni/JniBaseTypes.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {
namespace io {

class JBitmapTarget : public facebook::jni::JavaClass<JBitmapTarget> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/BitmapTarget;";
  jboolean makeBitmapArgb(int width, int height);
  facebook::jni::local_ref<jni::JBitmap> getBitmap();
};

class JniBitmapImageSink : public io::IBitmapImageSink {
 private:
  facebook::jni::alias_ref<JBitmapTarget> jOutputBitmapTarget;

  bool hasSetConfiguration = false;
  image::Size imageSize = {0, 0};
  std::size_t currentScanline = 0;

 public:
  explicit JniBitmapImageSink(
      facebook::jni::alias_ref<JBitmapTarget> jOutputBitmapTarget);

  JniBitmapImageSink(const JniBitmapImageSink&) = delete;
  JniBitmapImageSink(JniBitmapImageSink&&) = default;

  ~JniBitmapImageSink() override = default;

  void setConfiguration(
      const image::Size& imageSize,
      const image::pixel::Specification& pixelSpecification) override;

 protected:
  void _write(const char* source, const std::size_t length) override;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
