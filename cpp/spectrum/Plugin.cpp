// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Plugin.h"

namespace facebook {
namespace spectrum {

void Plugin::insert(Plugin&& rhs) {
  rules.insert(
      rules.end(),
      std::make_move_iterator(rhs.rules.begin()),
      std::make_move_iterator(rhs.rules.end()));

  compressorProviders.insert(
      compressorProviders.end(),
      rhs.compressorProviders.begin(),
      rhs.compressorProviders.end());

  decompressorProviders.insert(
      decompressorProviders.end(),
      rhs.decompressorProviders.begin(),
      rhs.decompressorProviders.end());

  encodedImageFormatDetectorHandlers.insert(
      encodedImageFormatDetectorHandlers.end(),
      rhs.encodedImageFormatDetectorHandlers.begin(),
      rhs.encodedImageFormatDetectorHandlers.end());
}

} // namespace spectrum
} // namespace facebook
