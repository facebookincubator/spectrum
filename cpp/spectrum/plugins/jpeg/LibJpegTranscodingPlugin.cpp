// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegTranscodingPlugin.h"

#include <spectrum/Rule.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/plugins/jpeg/LibJpegCompressor.h>
#include <spectrum/plugins/jpeg/LibJpegDecompressor.h>
#include <spectrum/plugins/jpeg/LibJpegLosslessRotateAndCropRecipe.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

namespace {
inline codecs::CompressorProvider::Factory makeLibJpegCompressorFactory() {
  return [](const codecs::CompressorOptions& options)
             -> std::unique_ptr<codecs::ICompressor> {
    return std::make_unique<LibJpegCompressor>(options);
  };
}

inline codecs::DecompressorProvider::Factory makeLibJpegDecompressorFactory() {
  return [](io::IImageSource& source,
            const folly::Optional<image::Ratio>& samplingRatio,
            const Configuration& configuration) {
    return std::make_unique<LibJpegDecompressor>(
        source, configuration, samplingRatio);
  };
}

image::pixel::Specification pixelSpecificationNarrower(
    const image::pixel::Specification& pixelSpecification) {
  if (pixelSpecification.colorModel == image::pixel::colormodels::Gray) {
    return image::pixel::specifications::Gray;
  } else {
    return image::pixel::specifications::RGB;
  }
}

codecs::CompressorProvider makeLibJpegCompressorProvider() {
  return {
      .format = image::formats::Jpeg,
      .supportsSettingMetadata = true,
      .supportedChromaSamplingModes =
          {
              image::ChromaSamplingMode::S420,
              image::ChromaSamplingMode::S422,
              image::ChromaSamplingMode::S444,
          },
      .pixelSpecificationNarrower = &pixelSpecificationNarrower,
      .compressorFactory = makeLibJpegCompressorFactory(),
  };
}

codecs::DecompressorProvider makeLibJpegDecompressorProvider() {
  return {
      .format = image::formats::Jpeg,
      .supportedSamplingRatios =
          {
              {1, 8},
              {2, 8},
              {3, 8},
              {4, 8},
              {5, 8},
              {6, 8},
              {7, 8},
              {8, 8},
              {9, 8},
              {10, 8},
              {11, 8},
              {12, 8},
              {13, 8},
              {14, 8},
              {15, 8},
              {16, 8},
          },
      .decompressorFactory = makeLibJpegDecompressorFactory(),
  };
}

Rule makeLibJpegLosslessRotateCropTranscodeRule() {
  return Rule{
      .name = "libjpeg_lossless_rotate_and_crop",
      .allowedInputFormats = {image::formats::Jpeg},
      .allowedOutputFormats = {image::formats::Jpeg},
      .requiresEqualInputOutputFormat = false,
      .isPassthrough = true,
      .cropSupport = Rule::CropSupport::Approximate,
      .resizeSupport = Rule::ResizeSupport::None,
      .rotateSupport = Rule::RotateSupport::MultipleOf90,
      .recipeFactory =
          []() {
            return std::make_unique<LibJpegLosslessRotateAndCropRecipe>();
          },
  };
}
} // namespace

Plugin makeTranscodingPlugin() {
  auto plugin = Plugin{};
  plugin.rules.push_back(makeLibJpegLosslessRotateCropTranscodeRule());
  plugin.decompressorProviders.push_back(makeLibJpegDecompressorProvider());
  plugin.compressorProviders.push_back(makeLibJpegCompressorProvider());
  return plugin;
}

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
