// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpectrumException.h"

namespace facebook {
namespace spectrum {

facebook::jni::local_ref<JSpectrumException> JSpectrumException::fromNative(
    const SpectrumException& exception) {
  return newInstance(
      exception.name,
      exception.message.hasValue() ? *exception.message : exception.name,
      exception.location,
      exception.description);
}

} // namespace spectrum
} // namespace facebook
