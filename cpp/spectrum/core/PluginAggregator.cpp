// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "PluginAggregator.h"

#include <spectrum/codecs/EncodedImageFormatDetectorHandlers.h>
#include <spectrum/codecs/bitmap/BitmapCompressor.h>
#include <spectrum/codecs/bitmap/BitmapDecompressor.h>
#include <spectrum/core/recipes/BaseRecipe.h>
#include <spectrum/core/recipes/CopyRecipe.h>

namespace facebook {
namespace spectrum {
namespace core {

PluginAggregator::PluginAggregator(std::vector<Plugin>&& plugins)
    : Plugin({
          .rules = {recipes::CopyRecipe::makeRule()},
          .compressorProviders =
              {codecs::bitmap::BitmapCompressor::makeProvider()},
          .decompressorProviders =
              {codecs::bitmap::BitmapDecompressor::makeProvider()},
          .encodedImageFormatDetectorHandlers =
              codecs::makeAllImageFormatDetectorHandlers(),
      }) {
  for (auto& plugin : plugins) {
    insert(std::move(plugin));
  }

  rules.push_back(core::recipes::BaseRecipe::makeRule());
}

} // namespace core
} // namespace spectrum
} // namespace facebook
