// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibAvifTranscodingPlugin.h"

#include <spectrum/Rule.h>
#include <spectrum/Spectrum.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/image/Format.h>
#include <spectrum/plugins/avif/AvifDecompressor.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {

namespace {

inline codecs::DecompressorProvider::Factory makeAvifDecompressorFactory() {
  return [](io::IImageSource& source,
            const folly::Optional<image::Ratio>& /* unused */,
            const Configuration& /* unused */) {
    return std::make_unique<AvifDecompressor>(source);
  };
}

codecs::DecompressorProvider makeAvifDecompressorProvider() {
  return {
      .format = image::formats::Avif,
      .supportedSamplingRatios = {},
      .decompressorFactory = makeAvifDecompressorFactory(),
  };
}

} // namespace

Plugin makeTranscodingPlugin() {
  auto plugin = Plugin{};
  plugin.decompressorProviders.push_back(makeAvifDecompressorProvider());
  return plugin;
}

} // namespace avif
} // namespace plugins
} // namespace spectrum
} // namespace facebook
