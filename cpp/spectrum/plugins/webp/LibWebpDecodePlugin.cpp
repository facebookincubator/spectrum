// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibWebpDecodePlugin.h"

#include <spectrum/Rule.h>
#include <spectrum/Spectrum.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/plugins/webp/LibWebpDecompressor.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace webp {

namespace {

inline codecs::DecompressorProvider::Factory makeLibWebpDecompressorFactory() {
  return [](io::IImageSource& source,
            const folly::Optional<image::Ratio>& samplingRatio,
            const Configuration& /* unused */) {
    return std::make_unique<LibWebpDecompressor>(source, samplingRatio);
  };
}

codecs::DecompressorProvider makeLibWebpDecompressorProvider() {
  return {
      .format = image::formats::Webp,
      .supportedSamplingRatios = {},
      .decompressorFactory = makeLibWebpDecompressorFactory(),
  };
}

} // namespace

Plugin makeDecodePlugin() {
  auto plugin = Plugin{};
  plugin.decompressorProviders.push_back(makeLibWebpDecompressorProvider());
  return plugin;
}

} // namespace webp
} // namespace plugins
} // namespace spectrum
} // namespace facebook
