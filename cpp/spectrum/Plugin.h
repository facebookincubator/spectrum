// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Rule.h>
#include <spectrum/codecs/CompressorProvider.h>
#include <spectrum/codecs/DecompressorProvider.h>
#include <spectrum/codecs/EncodedImageFormatDetector.h>

#include <vector>

namespace facebook {
namespace spectrum {

/**
 * Represents an Spectrum plugin which comprehends rules, request transformers,
 * compressor and decompressor providers.
 */
struct Plugin {
  std::vector<Rule> rules;
  std::vector<codecs::CompressorProvider> compressorProviders;
  std::vector<codecs::DecompressorProvider> decompressorProviders;
  std::vector<codecs::EncodedImageFormatDetectorHandler>
      encodedImageFormatDetectorHandlers;

  void insert(Plugin&& rhs);
};

} // namespace spectrum
} // namespace facebook
