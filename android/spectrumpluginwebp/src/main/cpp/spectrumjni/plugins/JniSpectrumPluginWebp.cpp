// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpectrumPluginWebp.h"

#include <spectrum/plugins/webp/LibWebpDecodePlugin.h>

#ifdef SPECTRUM_OSS
#include <spectrum/plugins/webp/LibWebpEncodePlugin.h>
#endif

namespace facebook {
namespace spectrum {
namespace plugins {

jlong JSpectrumPluginWebp::nativeCreatePlugin() {
  auto webpDecodePlugin = webp::makeDecodePlugin();
  const auto nativePlugin = new Plugin{
      std::move(webpDecodePlugin.rules),
      std::move(webpDecodePlugin.compressorProviders),
      std::move(webpDecodePlugin.decompressorProviders),
      std::move(webpDecodePlugin.encodedImageFormatDetectorHandlers)};

#ifdef SPECTRUM_OSS
  auto webpEncodePlugin = webp::makeEncodePlugin();
  nativePlugin->insert(std::move(webpEncodePlugin));
#endif

  static_assert(sizeof(void*) <= sizeof(jlong), "sizeof(void*) <= sizeof(jlong)");
  return reinterpret_cast<jlong>(nativePlugin);
}

facebook::jni::local_ref<JSpectrumPluginWebp::jhybriddata>
JSpectrumPluginWebp::initHybrid(facebook::jni::alias_ref<jclass>) {
  return makeCxxInstance();
}

void JSpectrumPluginWebp::registerNatives() {
  registerHybrid(
      {makeNativeMethod(
           "nativeCreatePlugin", JSpectrumPluginWebp::nativeCreatePlugin),
       makeNativeMethod("initHybrid", JSpectrumPluginWebp::initHybrid)});
}

} // namespace plugins
} // namespace spectrum
} // namespace facebook
