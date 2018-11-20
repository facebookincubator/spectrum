// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <fbjni/fbjni.h>
#include <spectrum/io/IEncodedImageSink.h>

#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {
namespace io {

/**
 * Wrapper for the standard java.io.OutputStream class that supports writing
 * from a Java byte array.
 */
class JOutputStream : public facebook::jni::JavaClass<JOutputStream> {
 public:
  static constexpr const char* kJavaDescriptor = "Ljava/io/OutputStream;";
  void write(jbyteArray array);
};

/**
 * An ImageSink that writes to a JOutputStream. It's the primary image sink to
 * use with a JNI binding.
 */
class JniOutputStreamImageSink : public io::IEncodedImageSink {
 private:
  facebook::jni::alias_ref<JOutputStream> jOutputStream;

 public:
  explicit JniOutputStreamImageSink(
      facebook::jni::alias_ref<JOutputStream> jOutputStream);

  JniOutputStreamImageSink(const JniOutputStreamImageSink&) = delete;
  JniOutputStreamImageSink(JniOutputStreamImageSink&&) = default;

  ~JniOutputStreamImageSink() override = default;

  void setConfiguration(
      const image::Size& imageSize,
      const image::pixel::Specification& pixelSpecification) override;

 protected:
  void _write(const char* src, const std::size_t len) override;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
