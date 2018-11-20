// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpectrumPluginPng.h"

#include <spectrum/plugins/png/LibPngTranscodingPlugin.h>

namespace facebook {
namespace spectrum {
namespace plugins {

jlong JSpectrumPluginPng::nativeCreatePlugin() {
  auto pngPlugin = png::makeTranscodingPlugin();

  const auto nativePlugin = new Plugin{
      std::move(pngPlugin.rules),
      std::move(pngPlugin.compressorProviders),
      std::move(pngPlugin.decompressorProviders),
      std::move(pngPlugin.encodedImageFormatDetectorHandlers)};

  static_assert(
          sizeof(void*) <= sizeof(jlong), "sizeof(void*) <= sizeof(jlong)");
  return reinterpret_cast<jlong>(nativePlugin);
}

facebook::jni::local_ref<JSpectrumPluginPng::jhybriddata>
JSpectrumPluginPng::initHybrid(facebook::jni::alias_ref<jclass>) {
  return makeCxxInstance();
}

void JSpectrumPluginPng::registerNatives() {
  registerHybrid(
      {makeNativeMethod(
           "nativeCreatePlugin", JSpectrumPluginPng::nativeCreatePlugin),
       makeNativeMethod("initHybrid", JSpectrumPluginPng::initHybrid)});
}

} // namespace plugins
} // namespace spectrum
} // namespace facebook
