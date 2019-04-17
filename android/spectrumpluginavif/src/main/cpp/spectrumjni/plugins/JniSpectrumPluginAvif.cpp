// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpectrumPluginAvif.h"

#include <spectrum/plugins/avif/LibAvifTranscodingPlugin.h>

namespace facebook {
namespace spectrum {
namespace plugins {

jlong JSpectrumPluginAvif::nativeCreatePlugin() {
  auto avifPlugin = avif::makeTranscodingPlugin();
  const auto nativePlugin = new Plugin{
      std::move(avifPlugin.rules),
      std::move(avifPlugin.compressorProviders),
      std::move(avifPlugin.decompressorProviders),
      std::move(avifPlugin.encodedImageFormatDetectorHandlers)};

  static_assert(
      sizeof(void*) <= sizeof(jlong), "sizeof(void*) <= sizeof(jlong)");
  return reinterpret_cast<jlong>(nativePlugin);
}

facebook::jni::local_ref<JSpectrumPluginAvif::jhybriddata>
JSpectrumPluginAvif::initHybrid(facebook::jni::alias_ref<jclass>) {
  return makeCxxInstance();
}

void JSpectrumPluginAvif::registerNatives() {
  registerHybrid(
      {makeNativeMethod(
           "nativeCreatePlugin", JSpectrumPluginAvif::nativeCreatePlugin),
       makeNativeMethod("initHybrid", JSpectrumPluginAvif::initHybrid)});
}

} // namespace plugins
} // namespace spectrum
} // namespace facebook
