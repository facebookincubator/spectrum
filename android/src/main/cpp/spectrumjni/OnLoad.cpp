// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrumjni/SpectrumJni.h>

#include <fbjni/fbjni.h>

jint JNI_OnLoad(JavaVM* vm, void*) {
  return facebook::jni::initialize(
      vm, [] { facebook::spectrum::jni::SpectrumJni::registerNatives(); });
}
