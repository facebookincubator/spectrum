// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibPngTranscodingPlugin.h"

#include <spectrum/Rule.h>
#include <spectrum/Spectrum.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/plugins/png/LibPngCompressor.h>
#include <spectrum/plugins/png/LibPngDecompressor.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace png {

namespace {

inline codecs::DecompressorProvider::Factory makeLibPngDecompressorFactory() {
  return [](io::IImageSource& source,
            const folly::Optional<image::Ratio>& samplingRatio,
            const Configuration& /* unused */) {
    return std::make_unique<LibPngDecompressor>(source, samplingRatio);
  };
}

image::pixel::Specification pixelSpecificationNarrower(
    const image::pixel::Specification& pixelSpecification) {
  if (pixelSpecification.colorModel == image::pixel::colormodels::Gray) {
    return image::pixel::specifications::Gray;
  } else {
    switch (pixelSpecification.alphaInfo) {
      case image::pixel::AlphaInfo::None:
      case image::pixel::AlphaInfo::SkipLast:
      case image::pixel::AlphaInfo::SkipFirst:
        return image::pixel::specifications::RGB;
      default:
        return image::pixel::specifications::ARGB;
    }
  }
}

codecs::DecompressorProvider makeLibPngDecompressorProvider() {
  return {
      .format = image::formats::Png,
      .supportedSamplingRatios = {},
      .decompressorFactory = makeLibPngDecompressorFactory(),
  };
}

codecs::CompressorProvider makeLibPngCompressorProvider() {
  return {
      .format = image::formats::Png,
      .supportsSettingMetadata = false,
      .supportedChromaSamplingModes =
          {
              image::ChromaSamplingMode::S420,
          },
      .pixelSpecificationNarrower = &pixelSpecificationNarrower,
      .compressorFactory =
          [](const codecs::CompressorOptions& options) {
            return std::make_unique<LibPngCompressor>(options);
          },
  };
}

} // namespace

Plugin makeTranscodingPlugin() {
  auto plugin = Plugin{};
  plugin.decompressorProviders.push_back(makeLibPngDecompressorProvider());
  plugin.compressorProviders.push_back(makeLibPngCompressorProvider());
  return plugin;
}

} // namespace png
} // namespace plugins
} // namespace spectrum
} // namespace facebook
