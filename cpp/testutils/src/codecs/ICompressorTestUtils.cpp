// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ICompressorTestUtils.h"

namespace facebook {
namespace spectrum {
namespace codecs {
namespace testutils {

void writeRgbData(ICompressor& compressor, const image::Size& imageSize) {
  for (std::uint32_t i = 0; i < imageSize.height; i++) {
    auto scanline = std::make_unique<image::Scanline>(
        image::pixel::specifications::RGB, imageSize.width);
    for (std::uint32_t j = 0; j < imageSize.width; j++) {
      const auto pixelDataBegin = scanline->dataAtPixel(j);

      // wildly distributed color to ensure enough entropy for large output
      pixelDataBegin[0] = static_cast<std::uint8_t>((i + j) % 256);
      pixelDataBegin[1] = static_cast<std::uint8_t>((i + 2 * j) % 256);
      pixelDataBegin[2] = static_cast<std::uint8_t>((i + 3 * j) % 256);
    }
    compressor.writeScanline(std::move(scanline));
  }
}

} // namespace testutils
} // namespace codecs
} // namespace spectrum
} // namespace facebook
