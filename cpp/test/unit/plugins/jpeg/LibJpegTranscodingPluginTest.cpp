// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/plugins/jpeg/LibJpegTranscodingPlugin.h>
#include <spectrum/Spectrum.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {
namespace test {

TEST(
    plugins_jpeg_LibJpegTranscodingPlugin,
    whenFetchingDecompressorProvider_thenSupportedSamplingRatiosAreCorrect) {
  auto plugin = makeTranscodingPlugin();

  ASSERT_FALSE(plugin.decompressorProviders.empty());

  if (!plugin.decompressorProviders.empty()) {
    auto& decompressorProvider = plugin.decompressorProviders[0];
    auto ratios = decompressorProvider.supportedSamplingRatios;
    ASSERT_EQ(16, ratios.size());
    for (int i = 1; i <= 16; i++) {
      auto ratio = ratios[i - 1];
      ASSERT_EQ(i, ratio.numerator);
      ASSERT_EQ(8, ratio.denominator);
    }
  }
}

} // namespace test
} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
