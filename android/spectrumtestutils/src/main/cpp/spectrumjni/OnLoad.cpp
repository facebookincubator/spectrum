// Copyright 2004-present Facebook. All Rights Reserved.

#include "JniInputStreamImageSourceTest.h"
#include "JniLoopBackTestHelper.h"
#include "JniOutputStreamImageSinkTest.h"
#include "JniSpectrumExceptionTest.h"

#include <fbjni/fbjni.h>

using namespace facebook::spectrum;

jint JNI_OnLoad(JavaVM* vm, void*) {
  return facebook::jni::initialize(vm, [] {
    // non-loopback tests
    jni::JniInputStreamImageSourceTest::registerNatives();
    jni::JniOutputStreamImageSinkTest::registerNatives();
    JniSpectrumExceptionTest::registerNatives();

    // loopback: image::
    image::JniImageColorTest::registerNatives();
    image::pixel::JniImagePixelSpecificationTest::registerNatives();
    image::JniImageSizeTest::registerNatives();
    image::JniImageFormatTest::registerNatives();
    image::JniImageOrientationTest::registerNatives();
    image::JniMetadataTest::registerNatives();
    image::JniChromaSamplingModeTest::registerNatives();
    image::JniImageSpecificationTest::registerNatives();
  });
}
