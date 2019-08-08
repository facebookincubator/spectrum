// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "BitmapPixelsLock.h"

#include <android/bitmap.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSource.h>
#include <spectrumjni/BitmapPixelsLock.h>
namespace facebook {
namespace spectrum {
namespace jni {

BitmapPixelsLock::BitmapPixelsLock(JNIEnv* env, jobject bitmap)
    : env_(env), bitmap_(bitmap), ptr_(nullptr), scanlineSizeBytes_(0) {
  AndroidBitmapInfo info;
  if (AndroidBitmap_getInfo(env_, bitmap, &info) !=
      ANDROID_BITMAP_RESULT_SUCCESS) {
    SPECTRUM_ERROR_CSTR(
        io::error::ImageSourceFailure, "failed_to_obtain_bitmap_info");
  }

  scanlineSizeBytes_ = info.stride;

  if (AndroidBitmap_lockPixels(env_, bitmap_, (void**)&ptr_) !=
      ANDROID_BITMAP_RESULT_SUCCESS) {
    SPECTRUM_ERROR_CSTR(
        io::error::ImageSourceFailure, "failed_to_obtain_pixel_lock");
  }
}

bool BitmapPixelsLock::releaseLock() {
  return releaseLock(true);
}

bool BitmapPixelsLock::releaseLock(const bool permitException) {
  if (!ptr_) {
    // already released the lock
    return false;
  }

  ptr_ = nullptr;

  if (AndroidBitmap_unlockPixels(env_, bitmap_) !=
      ANDROID_BITMAP_RESULT_SUCCESS) {
    if (permitException) {
      SPECTRUM_ERROR_CSTR(
          io::error::ImageSourceFailure, "failed_to_release_pixel_lock");
    } else {
      return false;
    }
  }

  return true;
}

BitmapPixelsLock::~BitmapPixelsLock() {
  const auto pendingException = env_->ExceptionOccurred();
  if (!pendingException) {
    releaseLock(false);
    return;
  }

  // unlockPixels by itself should not throw, but it can't run if
  // there's an exception pending
  env_->ExceptionClear();
  releaseLock(false);
  env_->Throw(pendingException);
}

} // namespace jni
} // namespace spectrum
} // namespace facebook
