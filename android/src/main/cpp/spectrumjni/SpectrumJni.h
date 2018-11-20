// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <fbjni/fbjni.h>

#include <spectrum/Spectrum.h>

#include <spectrumjni/JniBaseTypes.h>
#include <spectrumjni/JniConfiguration.h>
#include <spectrumjni/JniOptions.h>
#include <spectrumjni/JniSpectrumResult.h>
#include <spectrumjni/io/JniBitmapImageSink.h>
#include <spectrumjni/io/JniInputStreamImageSource.h>
#include <spectrumjni/io/JniOutputStreamImageSink.h>
#include <spectrumjni/plugins/JniSpectrumPlugin.h>

namespace facebook {
namespace spectrum {
namespace jni {

class SpectrumJni : public facebook::jni::HybridClass<SpectrumJni> {
 private:
  friend HybridBase;

  const Spectrum _spectrum;

 public:
  constexpr static auto kJavaDescriptor =
      "Lcom/facebook/spectrum/SpectrumHybrid;";

  SpectrumJni(
      const Configuration& configuration,
      const facebook::jni::alias_ref<
          facebook::jni::JArrayClass<plugins::JSpectrumPlugin::javaobject>>
          plugins);

  static facebook::jni::local_ref<jhybriddata> initHybrid(
      facebook::jni::alias_ref<jhybridobject> hybridObject,
      facebook::jni::alias_ref<JConfiguration> configuration,
      facebook::jni::alias_ref<
          facebook::jni::JArrayClass<plugins::JSpectrumPlugin::javaobject>>
          plugins);

  static void registerNatives();

  facebook::jni::local_ref<JSpectrumResult> nativeDecode(
      facebook::jni::alias_ref<io::JInputStream> jInputStream,
      facebook::jni::alias_ref<io::JBitmapTarget> jOutputBitmapTarget,
      facebook::jni::alias_ref<spectrum::JDecodeOptions> jDecodeOptions);

  facebook::jni::local_ref<JSpectrumResult> nativeEncode(
      facebook::jni::alias_ref<JBitmap> jInputBitmap,
      facebook::jni::alias_ref<io::JOutputStream> jOutputStream,
      facebook::jni::alias_ref<spectrum::JEncodeOptions> jEncodeOptions);

  facebook::jni::local_ref<JSpectrumResult> nativeTranscode(
      facebook::jni::alias_ref<io::JInputStream> jInputStream,
      facebook::jni::alias_ref<io::JOutputStream> jOutputStream,
      facebook::jni::alias_ref<spectrum::JTranscodeOptions> jTranscodeOptions);

  facebook::jni::local_ref<JSpectrumResult> nativeTransform(
      facebook::jni::alias_ref<JBitmap> jInputBitmap,
      facebook::jni::alias_ref<io::JBitmapTarget> jOutputBitmapTarget,
      facebook::jni::alias_ref<spectrum::JTransformOptions> jTransformOptions);
};

} // namespace jni
} // namespace spectrum
} // namespace facebook
