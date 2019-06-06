// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniInputStreamImageSourceTest.h"
#include "JniLoopBackTestHelper.h"
#include "JniOutputStreamImageSinkTest.h"
#include "JniCropRequirementTest.h"
#include "JniSpectrumExceptionTest.h"

#include <fbjni/fbjni.h>

using namespace facebook::spectrum;

jint JNI_OnLoad(JavaVM* vm, void*) {
  return facebook::jni::initialize(vm, [] {
    // non-loopback tests
    jni::JniInputStreamImageSourceTest::registerNatives();
    jni::JniOutputStreamImageSinkTest::registerNatives();
    requirements::JniCropRequirementTest::registerNatives();
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

    // loopback: requirements::
    requirements::JniEncodeRequirementTest::registerNatives();
    requirements::JniResizeRequirementTest::registerNatives();

    // loopback: jni::
    jni::JniConfigurationTest::registerNatives();

    // loopback: options::
    options::JniDecodeOptionsTest::registerNatives();
    options::JniEncodeOptionsTest::registerNatives();
    options::JniTranscodeOptionsTest::registerNatives();
    options::JniTransformOptionsTest::registerNatives();
    options::JniTransformationsTest::registerNatives();
  });
}
