// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <fbjni/fbjni.h>
#include <cstdint>

namespace facebook {
namespace spectrum {
namespace jni {

// helper class to access Android's bitmap data
class BitmapPixelsLock {
 private:
  JNIEnv* env_;
  jobject bitmap_;
  uint8_t* ptr_;
  uint32_t scanlineSizeBytes_;

  bool releaseLock(const bool permitException);

 public:
  BitmapPixelsLock(JNIEnv* env, jobject bitmap);

  ~BitmapPixelsLock();

  /**
   * Releases the bitmap lock if it's locked.
   * Returns true if the lock was released successfully, false if the lock has
   * been released already. Throws runtime_error if lock is taken and unable to
   * release the lock.
   */
  bool releaseLock();

  uint8_t* getPixelsPtr() {
    return ptr_;
  }

  uint32_t getScanlineSizeBytes() {
    return scanlineSizeBytes_;
  }
};

} // namespace jni
} // namespace spectrum
} // namespace facebook
