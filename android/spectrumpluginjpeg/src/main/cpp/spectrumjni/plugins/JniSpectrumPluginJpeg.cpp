// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpectrumPluginJpeg.h"

#include <spectrum/plugins/jpeg/LibJpegTranscodingPlugin.h>

namespace facebook {
namespace spectrum {
namespace plugins {

jlong JSpectrumPluginJpeg::nativeCreatePlugin() {
  auto jpegPlugin = jpeg::makeTranscodingPlugin();

  const auto nativePlugin = new Plugin{
      std::move(jpegPlugin.rules),
      std::move(jpegPlugin.compressorProviders),
      std::move(jpegPlugin.decompressorProviders),
      std::move(jpegPlugin.encodedImageFormatDetectorHandlers)};

  static_assert(
          sizeof(void*) <= sizeof(jlong), "sizeof(void*) <= sizeof(jlong)");
  return reinterpret_cast<jlong>(nativePlugin);
}

facebook::jni::local_ref<JSpectrumPluginJpeg::jhybriddata>
JSpectrumPluginJpeg::initHybrid(facebook::jni::alias_ref<jclass>) {
  return makeCxxInstance();
}

void JSpectrumPluginJpeg::registerNatives() {
  registerHybrid(
      {makeNativeMethod(
           "nativeCreatePlugin", JSpectrumPluginJpeg::nativeCreatePlugin),
       makeNativeMethod("initHybrid", JSpectrumPluginJpeg::initHybrid)});
}

} // namespace plugins
} // namespace spectrum
} // namespace facebook
