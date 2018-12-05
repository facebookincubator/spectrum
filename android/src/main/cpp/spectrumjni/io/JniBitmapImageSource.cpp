// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniBitmapImageSource.h"

#include <spectrum/io/IImageSource.h>
#include <spectrumjni/BitmapPixelsLock.h>
#include <spectrumjni/JniBaseTypes.h>

#include <android/bitmap.h>
#include <fbjni/fbjni.h>

#include <cmath>
#include <cstddef>
#include <cstring>
#include <exception>

namespace facebook {
namespace spectrum {
namespace io {
namespace {
image::Specification imageSpecificationFromBitmap(
    facebook::jni::alias_ref<jni::JBitmap> jBitmap) {
  SPECTRUM_ENFORCE_IF(!jBitmap);

  AndroidBitmapInfo bitmapInfo;
  const int rc = AndroidBitmap_getInfo(
      facebook::jni::Environment::current(), jBitmap.get(), &bitmapInfo);
  SPECTRUM_ERROR_FORMAT_IF(
      rc != ANDROID_BITMAP_RESULT_SUCCESS,
      io::error::ImageSourceFailure,
      "AndroidBitmap_getInfo_returned_%d",
      rc);
  SPECTRUM_ERROR_FORMAT_IF(
      bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888,
      io::error::ImageSourceFailure,
      "AndroidBitmap_unsupported_pixel_format_%d",
      bitmapInfo.format);

  return image::Specification{
      .size =
          image::Size{
              .width = bitmapInfo.width,
              .height = bitmapInfo.height,
          },
      .format = image::formats::Bitmap,
      .pixelSpecification = image::pixel::specifications::RGBA,
      .orientation = image::Orientation::Up,
      .metadata = {},
  };
}
} // namespace

JniBitmapImageSource::JniBitmapImageSource(
    facebook::jni::alias_ref<jni::JBitmap> jInputBitmap)
    : IBitmapImageSource(),
      _jInputBitmap(jInputBitmap),
      _imageSpecification(imageSpecificationFromBitmap(jInputBitmap)) {}

image::Specification JniBitmapImageSource::imageSpecification() const {
  return _imageSpecification;
}

std::size_t JniBitmapImageSource::getTotalBytesRead() const {
  return _offsetBytes;
}

std::size_t JniBitmapImageSource::read(
    char* const destination,
    const std::size_t length) {
  SPECTRUM_ERROR_CSTR_IF(
      !destination, io::error::ImageSourceFailure, "null_destination");

  //
  // prepare bitmap
  //

  AndroidBitmapInfo bitmapInfo;
  const int rc = AndroidBitmap_getInfo(
      facebook::jni::Environment::current(), _jInputBitmap.get(), &bitmapInfo);
  SPECTRUM_ERROR_FORMAT_IF(
      rc != ANDROID_BITMAP_RESULT_SUCCESS,
      io::error::ImageSourceFailure,
      "AndroidBitmap_getInfo_returned_%d",
      rc);

  jni::BitmapPixelsLock bmpLock(
      facebook::jni::Environment::current(), _jInputBitmap.get());
  uint8_t* pixelPtr = bmpLock.getPixelsPtr();
  SPECTRUM_ERROR_CSTR_IF(
      pixelPtr == nullptr,
      io::error::ImageSourceFailure,
      "failed_to_lock_bitmap");

  //
  // copy data
  //

  const std::size_t bitmapLengthBytes = bitmapInfo.height * bitmapInfo.stride;

  assert(bitmapLengthBytes >= _offsetBytes);
  const std::size_t bitmapRemainingBytes = bitmapLengthBytes - _offsetBytes;

  const auto bytesToCopy = std::min(length, bitmapRemainingBytes);
  memcpy(destination, pixelPtr + _offsetBytes, bytesToCopy);
  _offsetBytes += bytesToCopy;

  bmpLock.releaseLock();

  return bytesToCopy;
}

std::size_t JniBitmapImageSource::available() {
  AndroidBitmapInfo bitmapInfo;
  const int rc = AndroidBitmap_getInfo(
      facebook::jni::Environment::current(), _jInputBitmap.get(), &bitmapInfo);
  SPECTRUM_ERROR_FORMAT_IF(
      rc != ANDROID_BITMAP_RESULT_SUCCESS,
      io::error::ImageSourceFailure,
      "AndroidBitmap_getInfo_returned_%d",
      rc);

  const std::size_t bitmapLengthBytes = bitmapInfo.height * bitmapInfo.stride;
  return bitmapLengthBytes - _offsetBytes;
}

} // namespace io
} // namespace spectrum
} // namespace facebook
