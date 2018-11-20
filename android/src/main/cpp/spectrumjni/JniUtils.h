// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <fbjni/fbjni.h>
#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace jni {

template <typename NATIVE_TYPE, typename JAVA_TYPE>
folly::Optional<NATIVE_TYPE> nullableJavaObjectToOptional(
    const JAVA_TYPE& javaObject) {
  return javaObject
      ? folly::Optional<NATIVE_TYPE>(javaObject->toNative())
      : folly::Optional<NATIVE_TYPE>();
}

template <typename OPTIONAL_NATIVE_TYPE, typename JAVA_TYPE>
facebook::jni::local_ref<JAVA_TYPE> optionalToNullableJavaObject(
    const OPTIONAL_NATIVE_TYPE& optional) {
  return optional.hasValue() ? JAVA_TYPE::fromNative(optional.value())
                             : nullptr;
}

} // namespace jni
} // namespace spectrum
} // namespace facebook
