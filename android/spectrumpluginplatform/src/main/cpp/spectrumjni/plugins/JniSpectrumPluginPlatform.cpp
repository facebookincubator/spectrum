// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniSpectrumPluginPlatform.h"
#include "JniSpectrumPlatformDecompressor.h"

#include <spectrum/image/Specification.h>
#include <spectrum/Plugin.h>

namespace facebook {
namespace spectrum {
namespace plugins {

codecs::DecompressorProvider makeLibPlatformDecompressorProvider(const image::EncodedFormat& format) {
  return {
          .format = format,
          .supportedSamplingRatios = {},
          .decompressorFactory = [](io::IImageSource& source,
                                    const folly::Optional<image::Ratio>& samplingRatio,
                                    const Configuration& /* unused */) {
              return std::make_unique<platform::JniPlatformDecompressor>(source, samplingRatio);
          },
  };
}

jlong JSpectrumPluginPlatform::nativeCreatePlugin() {
  const auto plugin = new Plugin{};
  plugin->decompressorProviders.push_back(makeLibPlatformDecompressorProvider(image::formats::Gif));
  plugin->decompressorProviders.push_back(makeLibPlatformDecompressorProvider(image::formats::Heif));
  plugin->decompressorProviders.push_back(makeLibPlatformDecompressorProvider(image::formats::Jpeg));
  plugin->decompressorProviders.push_back(makeLibPlatformDecompressorProvider(image::formats::Png));
  plugin->decompressorProviders.push_back(makeLibPlatformDecompressorProvider(image::formats::Webp));

  static_assert(sizeof(void*) <= sizeof(jlong), "sizeof(void*) <= sizeof(jlong)");
  return reinterpret_cast<jlong>(plugin);
}

facebook::jni::local_ref<JSpectrumPluginPlatform::jhybriddata>
JSpectrumPluginPlatform::initHybrid(facebook::jni::alias_ref<jclass>) {
  return makeCxxInstance();
}

void JSpectrumPluginPlatform::registerNatives() {
  registerHybrid(
      {makeNativeMethod(
           "nativeCreatePlugin", JSpectrumPluginPlatform::nativeCreatePlugin),
       makeNativeMethod("initHybrid", JSpectrumPluginPlatform::initHybrid)});
}

} // namespace plugins
} // namespace spectrum
} // namespace facebook
