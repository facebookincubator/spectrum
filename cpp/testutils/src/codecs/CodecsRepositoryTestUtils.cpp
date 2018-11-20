// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "CodecsRepositoryTestUtils.h"

#include <spectrum/testutils/codecs/FakeDecompressor.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace testutils {

Repository makeCodecRepositoryWithFakeDecompressor(
    const image::Size& decompressorImageSize,
    const image::pixel::Specification& decompressorPixelSpecification) {
  auto decompressorProvider = DecompressorProvider{
      .format = image::formats::Jpeg,
      .supportedSamplingRatios = {},
      .decompressorFactory =
          [decompressorImageSize, decompressorPixelSpecification](
              io::IImageSource& source,
              const folly::Optional<image::Ratio>& samplingRatio,
              const Configuration& configuration) {
            return std::make_unique<FakeDecompressor>(
                decompressorImageSize, decompressorPixelSpecification);
          }};

  return Repository{{decompressorProvider}, {}};
}

} // namespace testutils
} // namespace codecs
} // namespace spectrum
} // namespace facebook
