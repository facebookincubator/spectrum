// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpectrumResult.h"

#include <spectrum/Result.h>

#include <spectrumjni/image/JniSpecification.h>

#include <fbjni/fbjni.h>

#include <cassert>
#include <cstddef>

namespace facebook {
namespace spectrum {

facebook::jni::local_ref<JSpectrumResult> translate(const Result& result) {
  return JSpectrumResult::make(
      facebook::jni::make_jstring(result.ruleName),
      image::JSpecification::fromNative(result.inputImageSpecification),
      image::JSpecification::fromNative(result.outputImageSpecification),
      result.totalBytesRead,
      result.totalBytesWritten);
}

} // namespace spectrum
} // namespace facebook
