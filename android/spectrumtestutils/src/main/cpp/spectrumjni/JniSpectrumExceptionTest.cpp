// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpectrumExceptionTest.h"

namespace facebook {
namespace spectrum {

facebook::jni::local_ref<JniSpectrumExceptionTest::jhybriddata>
JniSpectrumExceptionTest::initHybrid(
    facebook::jni::alias_ref<JniSpectrumExceptionTest::jhybridobject>) {
  return makeCxxInstance();
}

void JniSpectrumExceptionTest::throwSpectrumException(
    const std::string name,
    const std::string message,
    const std::string function,
    const int line) {
  const auto exception =
      SpectrumException(name, message, function.c_str(), line);
  throw facebook::jni::JniException(JSpectrumException::fromNative(exception));
}

void JniSpectrumExceptionTest::throwSpectrumExceptionWithoutMessage(
    const std::string name,
    const std::string function,
    const int line) {
  const auto exception =
      SpectrumException(name, folly::none, function.c_str(), line);
  throw facebook::jni::JniException(JSpectrumException::fromNative(exception));
}

void JniSpectrumExceptionTest::registerNatives() {
  registerHybrid({
      makeNativeMethod("initHybrid", JniSpectrumExceptionTest::initHybrid),
      makeNativeMethod(
          "throwSpectrumException",
          JniSpectrumExceptionTest::throwSpectrumException),
      makeNativeMethod(
          "throwSpectrumExceptionWithoutMessage",
          JniSpectrumExceptionTest::throwSpectrumExceptionWithoutMessage),
  });
}

} // namespace spectrum
} // namespace facebook
