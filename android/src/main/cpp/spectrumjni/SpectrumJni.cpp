// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "SpectrumJni.h"

#include <spectrum/Options.h>
#include <spectrum/Spectrum.h>
#include <spectrum/SpectrumException.h>
#include <spectrum/io/IImageSink.h>

#include <spectrumjni/JniBaseTypes.h>
#include <spectrumjni/JniSpectrumException.h>
#include <spectrumjni/JniSpectrumResult.h>
#include <spectrumjni/io/JniBitmapImageSink.h>
#include <spectrumjni/io/JniBitmapImageSource.h>
#include <spectrumjni/io/JniInputStreamImageSource.h>
#include <spectrumjni/io/JniOutputStreamImageSink.h>

#include <fbjni/fbjni.h>
#include <exception>

namespace facebook {
namespace spectrum {
namespace jni {
namespace {

Spectrum makeSpectrum(
    const Configuration& configuration,
    facebook::jni::alias_ref<
        facebook::jni::JArrayClass<plugins::JSpectrumPlugin::javaobject>>
        plugins) {
  std::vector<Plugin> nativePlugins;

  for (std::size_t i = 0; i < plugins->size(); i++) {
    auto plugin = plugins->getElement(i);
    Plugin* nativePluginPtr = reinterpret_cast<Plugin*>(plugin->getPlugin());

    nativePlugins.push_back(
        Plugin{std::move(nativePluginPtr->rules),
               std::move(nativePluginPtr->compressorProviders),
               std::move(nativePluginPtr->decompressorProviders),
               std::move(nativePluginPtr->encodedImageFormatDetectorHandlers)});

    delete nativePluginPtr;
  }

  return Spectrum(std::move(nativePlugins), configuration);
}

} // namespace

//
// SpectrumJni
//

SpectrumJni::SpectrumJni(
    const Configuration& configuration,
    const facebook::jni::alias_ref<
        facebook::jni::JArrayClass<plugins::JSpectrumPlugin::javaobject>>
        plugins)
    : _spectrum(makeSpectrum(configuration, plugins)) {}

facebook::jni::local_ref<SpectrumJni::jhybriddata> SpectrumJni::initHybrid(
    facebook::jni::alias_ref<jhybridobject> hybridObject,
    facebook::jni::alias_ref<JConfiguration> configuration,
    const facebook::jni::alias_ref<
        facebook::jni::JArrayClass<plugins::JSpectrumPlugin::javaobject>>
        plugins) {
  return makeCxxInstance(configuration->toNative(), plugins);
}

facebook::jni::local_ref<JSpectrumResult> SpectrumJni::nativeDecode(
    facebook::jni::alias_ref<io::JInputStream> jInputStream,
    facebook::jni::alias_ref<io::JBitmapTarget> jOutputBitmapTarget,
    facebook::jni::alias_ref<spectrum::JDecodeOptions> jDecodeOptions) {
  try {
    auto source = io::JniInputStreamImageSource(jInputStream);
    auto sink = io::JniBitmapImageSink(jOutputBitmapTarget);

    const auto result =
        _spectrum.decode(source, sink, jDecodeOptions->toNative());
    return translate(result);
  } catch (const SpectrumException& exception) {
    throw facebook::jni::JniException(
        JSpectrumException::fromNative(exception));
  }
}

facebook::jni::local_ref<JSpectrumResult> SpectrumJni::nativeEncode(
    facebook::jni::alias_ref<JBitmap> jInputBitmap,
    facebook::jni::alias_ref<io::JOutputStream> jOutputStream,
    facebook::jni::alias_ref<spectrum::JEncodeOptions> jEncodeOptions) {
  try {
    auto source = io::JniBitmapImageSource(jInputBitmap);
    auto sink = io::JniOutputStreamImageSink(jOutputStream);

    const auto result =
        _spectrum.encode(source, sink, jEncodeOptions->toNative());
    return translate(result);
  } catch (const SpectrumException& exception) {
    throw facebook::jni::JniException(
        JSpectrumException::fromNative(exception));
  }
}

facebook::jni::local_ref<JSpectrumResult> SpectrumJni::nativeTranscode(
    facebook::jni::alias_ref<io::JInputStream> jInputStream,
    facebook::jni::alias_ref<io::JOutputStream> jOutputStream,
    facebook::jni::alias_ref<spectrum::JTranscodeOptions> jTranscodeOptions) {
  try {
    auto source = io::JniInputStreamImageSource(jInputStream);
    auto sink = io::JniOutputStreamImageSink(jOutputStream);

    const auto result =
        _spectrum.transcode(source, sink, jTranscodeOptions->toNative());
    return translate(result);
  } catch (const SpectrumException& exception) {
    throw facebook::jni::JniException(
        JSpectrumException::fromNative(exception));
  }
}

facebook::jni::local_ref<JSpectrumResult> SpectrumJni::nativeTransform(
    facebook::jni::alias_ref<JBitmap> jInputBitmap,
    facebook::jni::alias_ref<io::JBitmapTarget> jOutputBitmapTarget,
    facebook::jni::alias_ref<spectrum::JTransformOptions> jTransformOptions) {
  try {
    auto source = io::JniBitmapImageSource(jInputBitmap);
    auto sink = io::JniBitmapImageSink(jOutputBitmapTarget);

    const auto result =
        _spectrum.transform(source, sink, jTransformOptions->toNative());
    return translate(result);
  } catch (const SpectrumException& exception) {
    throw facebook::jni::JniException(
        JSpectrumException::fromNative(exception));
  }
}

void SpectrumJni::registerNatives() {
  registerHybrid(
      {makeNativeMethod("initHybrid", SpectrumJni::initHybrid),
       makeNativeMethod("nativeDecode", SpectrumJni::nativeDecode),
       makeNativeMethod("nativeEncode", SpectrumJni::nativeEncode),
       makeNativeMethod("nativeTranscode", SpectrumJni::nativeTranscode),
       makeNativeMethod("nativeTransform", SpectrumJni::nativeTransform)});
}

} // namespace jni
} // namespace spectrum
} // namespace facebook
