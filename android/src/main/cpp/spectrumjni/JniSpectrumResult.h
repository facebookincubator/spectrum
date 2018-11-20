// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Result.h>

#include <spectrumjni/image/JniSpecification.h>

#include <fbjni/fbjni.h>

#include <cstddef>

namespace facebook {
namespace spectrum {

class JSpectrumResult : public facebook::jni::JavaClass<JSpectrumResult> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/SpectrumResult;";

  static facebook::jni::local_ref<JSpectrumResult> make(
      facebook::jni::local_ref<facebook::jni::JString> transcodeRuleName,
      facebook::jni::local_ref<image::JSpecification> inputImageSpecification,
      facebook::jni::local_ref<image::JSpecification> outputImageSpecification,
      jlong totalBytesRead,
      jlong totalBytesWritten) {
    return newInstance(
        transcodeRuleName,
        inputImageSpecification,
        outputImageSpecification,
        totalBytesRead,
        totalBytesWritten);
  }
};

/**
 * This method takes a facebook::spectrum::Result (C++) and creates a
 * com.facebook.spectrum.SpectrumResult (Java) with the equivalent settings
 */
facebook::jni::local_ref<JSpectrumResult> translate(const Result& result);

} // namespace spectrum
} // namespace facebook
