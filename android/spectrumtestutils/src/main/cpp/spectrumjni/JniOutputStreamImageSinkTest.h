// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <fbjni/fbjni.h>
#include <spectrumjni/io/JniOutputStreamImageSink.h>

namespace facebook {
namespace spectrum {
namespace jni {

/**
 * Native counter-part for JniOutputStreamImageSinkTest.java
 */
class JniOutputStreamImageSinkTest
    : public facebook::jni::HybridClass<JniOutputStreamImageSinkTest> {
 private:
  friend HybridBase;

 public:
  constexpr static auto kJavaDescriptor =
      "Lcom/facebook/spectrum/JniOutputStreamImageSinkTest;";

  static facebook::jni::local_ref<jhybriddata> initHybrid(
      facebook::jni::alias_ref<jhybridobject>);

  /**
   * Writes the entire content of a byte array to the given output stream
   */
  void writeToOutputStream(
      facebook::jni::alias_ref<::facebook::jni::JArrayByte> array,
      facebook::jni::alias_ref<io::JOutputStream> jOutputStream);

  static void registerNatives();
};

} // namespace jni
} // namespace spectrum
} // namespace facebook
