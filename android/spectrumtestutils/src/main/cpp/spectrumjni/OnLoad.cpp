// Copyright 2004-present Facebook. All Rights Reserved.

#include "JniInputStreamImageSourceTest.h"
#include "JniOutputStreamImageSinkTest.h"
#include "JniSpectrumExceptionTest.h"

#include <fbjni/fbjni.h>

using namespace facebook::spectrum;

jint JNI_OnLoad(JavaVM* vm, void*) {
  return facebook::jni::initialize(vm, [] {
    jni::JniInputStreamImageSourceTest::registerNatives();
    jni::JniOutputStreamImageSinkTest::registerNatives();
    JniSpectrumExceptionTest::registerNatives();
  });
}
