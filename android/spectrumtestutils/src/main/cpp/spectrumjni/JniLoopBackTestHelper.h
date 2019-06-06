// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrumjni/JniConfiguration.h>
#include <spectrumjni/JniOptions.h>
#include <spectrumjni/JniTransformations.h>
#include <spectrumjni/image/JniFormat.h>
#include <spectrumjni/image/JniGeometry.h>
#include <spectrumjni/image/JniImageColor.h>
#include <spectrumjni/image/JniMetadata.h>
#include <spectrumjni/image/JniOrientation.h>
#include <spectrumjni/image/JniPixel.h>
#include <spectrumjni/image/JniSpecification.h>
#include <spectrumjni/requirements/JniEncode.h>
#include <spectrumjni/requirements/JniResize.h>

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {

#define SPECTRUM_MAKE_LOOPBACK_CLASS_WITH_INPUT_OUTPUT(                      \
    ClassName, InputJniClass, OutputJniClass, TestClassDescriptor)           \
  class ClassName : public facebook::jni::HybridClass<ClassName> {           \
   public:                                                                   \
    constexpr static auto kJavaDescriptor = TestClassDescriptor;             \
                                                                             \
    static facebook::jni::local_ref<jhybriddata> initHybrid(                 \
        facebook::jni::alias_ref<jhybridobject>) {                           \
      return makeCxxInstance();                                              \
    }                                                                        \
                                                                             \
    facebook::jni::local_ref<OutputJniClass> loopback(                       \
        facebook::jni::alias_ref<InputJniClass> jObject) {                   \
      const auto twin = jObject->toNative();                                 \
      return OutputJniClass::fromNative(twin);                               \
    }                                                                        \
                                                                             \
    static void registerNatives() {                                          \
      registerHybrid({makeNativeMethod("initHybrid", ClassName::initHybrid), \
                      makeNativeMethod("loopback", ClassName::loopback)});   \
    }                                                                        \
  };

#define SPECTRUM_MAKE_LOOPBACK_CLASS(ClassName, JniClass, TestClassDescriptor) \
  SPECTRUM_MAKE_LOOPBACK_CLASS_WITH_INPUT_OUTPUT(                              \
      ClassName, JniClass, JniClass, TestClassDescriptor)

namespace image {
SPECTRUM_MAKE_LOOPBACK_CLASS(
    JniImageColorTest,
    JColor,
    "Lcom/facebook/spectrum/image/JniImageColorTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS(
    JniImageFormatTest,
    JFormat,
    "Lcom/facebook/spectrum/image/JniImageFormatTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS(
    JniImageSizeTest,
    JSize,
    "Lcom/facebook/spectrum/image/JniImageSizeTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS(
    JniImageOrientationTest,
    JOrientation,
    "Lcom/facebook/spectrum/image/JniImageOrientationTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS(
    JniMetadataTest,
    JMetadata,
    "Lcom/facebook/spectrum/image/JniImageMetadataTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS(
    JniChromaSamplingModeTest,
    JChromaSamplingMode,
    "Lcom/facebook/spectrum/image/JniImageChromaSamplingModeTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS(
    JniImageSpecificationTest,
    JSpecification,
    "Lcom/facebook/spectrum/image/JniImageSpecificationTest;");

namespace pixel {

SPECTRUM_MAKE_LOOPBACK_CLASS(
    JniImagePixelSpecificationTest,
    JSpecification,
    "Lcom/facebook/spectrum/image/JniImagePixelSpecificationTest;");
}

} // namespace image

namespace requirements {

SPECTRUM_MAKE_LOOPBACK_CLASS(
        JniEncodeRequirementTest,
        JEncode,
        "Lcom/facebook/spectrum/requirements/JniEncodeRequirementTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS(
        JniResizeRequirementTest,
        JResize,
        "Lcom/facebook/spectrum/requirements/JniResizeRequirementTest;");

} // namespace requirements

namespace jni {

SPECTRUM_MAKE_LOOPBACK_CLASS(
        JniConfigurationTest,
        JConfiguration,
        "Lcom/facebook/spectrum/types/JniConfigurationTest;");

} // namespace jni

namespace options {

SPECTRUM_MAKE_LOOPBACK_CLASS(
        JniTransformationsTest,
        JTransformations,
        "Lcom/facebook/spectrum/options/JniTransformationsTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS_WITH_INPUT_OUTPUT(
        JniDecodeOptionsTest,
        JDecodeOptions,
        JOptions,
        "Lcom/facebook/spectrum/options/JniDecodeOptionsTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS_WITH_INPUT_OUTPUT(
        JniEncodeOptionsTest,
        JEncodeOptions,
        JOptions,
        "Lcom/facebook/spectrum/options/JniEncodeOptionsTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS_WITH_INPUT_OUTPUT(
        JniTranscodeOptionsTest,
        JTranscodeOptions,
        JOptions,
        "Lcom/facebook/spectrum/options/JniTranscodeOptionsTest;");

SPECTRUM_MAKE_LOOPBACK_CLASS_WITH_INPUT_OUTPUT(
        JniTransformOptionsTest,
        JTransformOptions,
        JOptions,
        "Lcom/facebook/spectrum/options/JniTransformOptionsTest;");

} // namespace jni

#undef SPECTRUM_MAKE_LOOPBACK_CLASS

} // namespace spectrum
} // namespace facebook
