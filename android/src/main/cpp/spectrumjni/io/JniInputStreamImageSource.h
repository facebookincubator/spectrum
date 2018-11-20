// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <fbjni/fbjni.h>
#include <spectrum/io/IEncodedImageSource.h>

#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {
namespace io {

/**
 * Wrapper for the standard java.io.inputStream class that supports reading into
 * a Java byte array.
 */
class JInputStream : public facebook::jni::JavaClass<JInputStream> {
 public:
  static constexpr const char* kJavaDescriptor = "Ljava/io/InputStream;";
  jint read(jbyteArray array);
  jint available();
};

/**
 * An ImageSource that reads from a JInputStream. It's the primary image source
 * to use with a JNI binding.
 */
class JniInputStreamImageSource : public io::IEncodedImageSource {
 private:
  facebook::jni::alias_ref<JInputStream> jInputStream;
  std::size_t totalBytesRead = 0;

 public:
  explicit JniInputStreamImageSource(
      facebook::jni::alias_ref<JInputStream> jInputStream);

  JniInputStreamImageSource(const JniInputStreamImageSource&) = delete;
  JniInputStreamImageSource(JniInputStreamImageSource&&) = default;

  ~JniInputStreamImageSource() override = default;

  std::size_t read(char* const destination, const std::size_t length) override;
  std::size_t getTotalBytesRead() const override {
    return totalBytesRead;
  }
  std::size_t available() override;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
