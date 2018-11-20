// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibWebpEncodePlugin.h"

#include <spectrum/Rule.h>
#include <spectrum/Spectrum.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/plugins/webp/LibWebpCompressor.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace webp {

namespace {
inline codecs::CompressorProvider::Factory makeLibWebpCompressorFactory() {
  return [](const codecs::CompressorOptions& options) {
    return std::make_unique<LibWebpCompressor>(options);
  };
}

codecs::CompressorProvider makeLibWebpCompressorProvider() {
  return {
      .format = image::formats::Webp,
      .supportsSettingMetadata = false,
      .supportedChromaSamplingModes =
          {
              image::ChromaSamplingMode::S420,
          },
      .pixelSpecificationNarrower =
          [](const image::pixel::Specification& /* unused */) {
            return image::pixel::specifications::RGBA;
          },
      .compressorFactory = makeLibWebpCompressorFactory(),
  };
}
} // namespace

Plugin makeEncodePlugin() {
  auto plugin = Plugin{};
  plugin.compressorProviders.push_back(makeLibWebpCompressorProvider());
  return plugin;
}

} // namespace webp
} // namespace plugins
} // namespace spectrum
} // namespace facebook
