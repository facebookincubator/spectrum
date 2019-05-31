// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <fbjni/fbjni.h>
#include <spectrumjni/io/JniInputStreamImageSource.h>

namespace facebook {
namespace spectrum {
namespace jni {

/**
 * Native counter-part for JniInputStreamImageSourceTest.java
 */
class JniInputStreamImageSourceTest
    : public facebook::jni::HybridClass<JniInputStreamImageSourceTest> {
 private:
  friend HybridBase;

 public:
  constexpr static auto kJavaDescriptor =
      "Lcom/facebook/spectrum/JniInputStreamImageSourceTest;";

  static facebook::jni::local_ref<jhybriddata> initHybrid(
      facebook::jni::alias_ref<jhybridobject>);

  /**
   * Reads the entire content of an input stream to a byte array
   */
  facebook::jni::local_ref<facebook::jni::JArrayByte> readFromInputStream(
      facebook::jni::alias_ref<io::JInputStream> jinputstream);

  jint getAvailableFromInputStream(
      facebook::jni::alias_ref<io::JInputStream> jinputstream);

  static void registerNatives();
};

} // namespace jni
} // namespace spectrum
} // namespace facebook
