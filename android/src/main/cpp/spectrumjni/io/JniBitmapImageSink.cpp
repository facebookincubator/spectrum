// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniBitmapImageSink.h"

#include <spectrum/io/IImageSink.h>
#include <spectrumjni/BitmapPixelsLock.h>
#include <spectrumjni/JniBaseTypes.h>

#include <android/bitmap.h>
#include <fbjni/fbjni.h>

#include <cstddef>
#include <cstring>
#include <exception>

namespace facebook {
namespace spectrum {
namespace io {

jboolean JBitmapTarget::makeBitmapArgb(int width, int height) {
  static const auto method =
      javaClassStatic()->getMethod<jboolean(int, int)>("createBitmapArgb");
  return method(self(), width, height);
}

facebook::jni::local_ref<jni::JBitmap> JBitmapTarget::getBitmap() {
  static const auto method =
      javaClassStatic()->getMethod<facebook::jni::local_ref<jni::JBitmap>()>(
          "getBitmap");
  return method(self());
}

JniBitmapImageSink::JniBitmapImageSink(
    facebook::jni::alias_ref<JBitmapTarget> jOutputBitmapTarget)
    : jOutputBitmapTarget(jOutputBitmapTarget) {
  assert(this->jOutputBitmapTarget);
}

void JniBitmapImageSink::setConfiguration(
    const image::Size& imageSize,
    const image::pixel::Specification& pixelSpecification) {
  this->hasSetConfiguration = true;
  this->imageSize = imageSize;

  if (pixelSpecification == image::pixel::specifications::RGBA) {
    jOutputBitmapTarget->makeBitmapArgb(imageSize.width, imageSize.height);
  } else {
    SPECTRUM_ERROR_CSTR(
        io::error::ImageSinkFailure,
        "unsupported_pixel_type_for_android_bitmap");
  }
}

void JniBitmapImageSink::_write(const char* source, const std::size_t length) {
  SPECTRUM_ERROR_CSTR_IF(!source, io::error::ImageSinkFailure, "null_source");
  SPECTRUM_ERROR_CSTR_IF(
      !hasSetConfiguration,
      io::error::ImageSinkFailure,
      "no_set_configuration");
  SPECTRUM_ERROR_CSTR_IF(
      currentScanline >= imageSize.height,
      io::error::ImageSinkFailure,
      "currentScanline_above_imageSize.height");
  SPECTRUM_ERROR_CSTR_IF(
      length !=
          imageSize.width * image::pixel::specifications::RGBA.bytesPerPixel,
      io::error::ImageSinkFailure,
      "length_imageSize.width_mismatch");

  facebook::jni::local_ref<jni::JBitmap> bitmap =
      jOutputBitmapTarget->getBitmap();

  jni::BitmapPixelsLock bmpLock(
      facebook::jni::Environment::current(), bitmap.get());
  uint8_t* pixelPtr = bmpLock.getPixelsPtr();
  SPECTRUM_ERROR_CSTR_IF(
      pixelPtr == nullptr,
      io::error::ImageSinkFailure,
      "failed_to_lock_bitmap");

  //
  // copy data
  //

  const std::size_t offset = currentScanline * imageSize.width *
      image::pixel::specifications::RGBA.bytesPerPixel;
  memcpy(pixelPtr + offset, source, length);
  ++currentScanline;

  bmpLock.releaseLock();
}

} // namespace io
} // namespace spectrum
} // namespace facebook
