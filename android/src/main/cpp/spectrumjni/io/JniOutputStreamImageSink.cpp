// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniOutputStreamImageSink.h"

#include <fbjni/fbjni.h>

#include <cassert>

namespace facebook {
namespace spectrum {
namespace io {

//
// JOutputStream
//

void JOutputStream::write(jbyteArray array) {
  static auto writeMethod =
      javaClassStatic()->getMethod<void(jbyteArray)>("write");
  return writeMethod(self(), array);
}

//
// JniOutputStreamImageSink
//

JniOutputStreamImageSink::JniOutputStreamImageSink(
    facebook::jni::alias_ref<JOutputStream> jOutputStream)
    : IEncodedImageSink(), jOutputStream(jOutputStream) {
  SPECTRUM_ERROR_CSTR_IF(
      !jOutputStream.get(), io::error::ImageSinkFailure, "null_jOutputStream");
}

void JniOutputStreamImageSink::setConfiguration(
    const image::Size& /* unused */,
    const image::pixel::Specification& /* unused */) {
  // TODO 19957147: pass up image size and pixel type for encoded streams
}

void JniOutputStreamImageSink::_write(const char* src, const std::size_t len) {
  if (len == 0) {
    return;
  }

  // allocate temp byte array on java heap
  auto arr = facebook::jni::JArrayByte::newArray(len);

  // copy read bytes to java heap from destination: for this we reinterpret the
  // destination as signed chars which have the same underlying byte size
  static_assert(
      sizeof(signed char) == sizeof(char),
      "wtf signed char and char differ in size");
  arr->setRegion(0, len, reinterpret_cast<const signed char*>(src));

  // issue write on Output stream (this blocks)
  jOutputStream->write(arr.get());
}

} // namespace io
} // namespace spectrum
} // namespace facebook
