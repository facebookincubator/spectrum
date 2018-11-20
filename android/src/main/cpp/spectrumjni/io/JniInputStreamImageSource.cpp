// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniInputStreamImageSource.h"

#include <cassert>

#include <fbjni/fbjni.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSource.h>
namespace facebook {
namespace spectrum {
namespace io {

//
// JInputStream
//

jint JInputStream::read(jbyteArray array) {
  static auto readMethod =
      javaClassStatic()->getMethod<jint(jbyteArray)>("read");
  return readMethod(self(), array);
}

jint JInputStream::available() {
  static auto method = javaClassStatic()->getMethod<jint()>("available");
  return method(self());
}

//
// JniInputStreamImageSource
//

JniInputStreamImageSource::JniInputStreamImageSource(
    facebook::jni::alias_ref<JInputStream> jInputStream)
    : IEncodedImageSource(), jInputStream(jInputStream) {
  SPECTRUM_ERROR_CSTR_IF(
      !jInputStream.get(), io::error::ImageSourceFailure, "null_jInputStream");
}

static constexpr jint INPUT_STREAM_RESULT_END_OF_STREAM = -1;

std::size_t JniInputStreamImageSource::read(
    char* const destination,
    const std::size_t length) {
  // allocate temp byte array on java heap
  auto arr = facebook::jni::JArrayByte::newArray(length);

  // issue read on input stream (this blocks until end-of-stream or at least one
  // byte is read)
  jint bytesRead = jInputStream->read(arr.get());

  if (INPUT_STREAM_RESULT_END_OF_STREAM == bytesRead) {
    return 0;
  }

  // bytesRead must only be 0 if trying to read 0 bytes
  assert(length == 0 || bytesRead != 0);

  if (bytesRead > 0) {
    // copy read bytes from java heap to destination: for this we reinterpret
    // the destination as signed chars which have the same underlying byte size
    static_assert(
        sizeof(signed char) == sizeof(char),
        "wtf signed char and char differ in size");
    arr->getRegion(0, bytesRead, reinterpret_cast<signed char*>(destination));
  }

  totalBytesRead += bytesRead;

  return bytesRead;
}

std::size_t JniInputStreamImageSource::available() {
  return jInputStream->available();
}

} // namespace io
} // namespace spectrum
} // namespace facebook
