// Copyright 2004-present Facebook. All Rights Reserved.

#include "JniOutputStreamImageSinkTest.h"

#include <algorithm>
#include <exception>
#include <memory>
#include <vector>

#include <spectrumjni/io/JniOutputStreamImageSink.h>

namespace facebook {
namespace spectrum {
namespace jni {

facebook::jni::local_ref<JniOutputStreamImageSinkTest::jhybriddata>
JniOutputStreamImageSinkTest::initHybrid(
    facebook::jni::alias_ref<jhybridobject>) {
  return makeCxxInstance();
}

void JniOutputStreamImageSinkTest::writeToOutputStream(
    facebook::jni::alias_ref<::facebook::jni::JArrayByte> array,
    facebook::jni::alias_ref<io::JOutputStream> jOutputStream) {
  // wrap as image sink
  auto sink = std::make_shared<io::JniOutputStreamImageSink>(jOutputStream);

  auto pinned = array->pin();
  sink->write(reinterpret_cast<char*>(pinned.get()), pinned.size());
}

void JniOutputStreamImageSinkTest::registerNatives() {
  registerHybrid({
      makeNativeMethod("initHybrid", JniOutputStreamImageSinkTest::initHybrid),
      makeNativeMethod(
          "writeToOutputStream",
          JniOutputStreamImageSinkTest::writeToOutputStream),
  });
}

} // namespace jni
} // namespace spectrum
} // namespace facebook
