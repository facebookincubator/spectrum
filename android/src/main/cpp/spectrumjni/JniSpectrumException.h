// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/SpectrumException.h>

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {

class JSpectrumException
    : public facebook::jni::
          JavaClass<JSpectrumException, facebook::jni::JThrowable> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/SpectrumException;";

  static facebook::jni::local_ref<JSpectrumException> fromNative(
      const SpectrumException& exception);
};

} // namespace spectrum
} // namespace facebook
