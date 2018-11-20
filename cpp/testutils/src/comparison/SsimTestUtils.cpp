// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/testutils/comparison/SsimTestUtils.h>

#include <spectrum/io/FileImageSource.h>
#include <spectrum/plugins/jpeg/LibJpegDecompressor.h>

namespace facebook {
namespace spectrum {
namespace comparison {
namespace testutils {

std::unique_ptr<image::Scanline> sampleScanline(
    Rng& rng,
    const image::pixel::Specification& spec,
    const std::size_t width) {
  auto scanline = std::make_unique<image::Scanline>(spec, width);

  for (std::size_t x = 0; x < width; x += 1) {
    auto* data = scanline->dataAtPixel(x);
    for (std::size_t c = 0; c < spec.numberOfComponents(); c += 1) {
      data[c] = static_cast<std::uint8_t>(rng());
    }
  }

  return scanline;
}

std::vector<std::unique_ptr<image::Scanline>> sampleImage(
    Rng& rng,
    const image::pixel::Specification& spec,
    const image::Size size) {
  std::vector<std::unique_ptr<image::Scanline>> result;
  result.reserve(size.height);

  for (std::size_t y = 0; y < size.height; y += 1) {
    result.push_back(sampleScanline(rng, spec, size.width));
  }

  return result;
}

std::vector<std::unique_ptr<image::Scanline>> makeImageFill(
    const image::pixel::Specification& spec,
    const image::Size size,
    const std::vector<std::uint8_t>&& pixel) {
  EXPECT_EQ(spec.bytesPerPixel, pixel.size());

  std::vector<std::unique_ptr<image::Scanline>> result;
  result.reserve(size.height);

  for (std::size_t y = 0; y < size.height; y += 1) {
    auto scanline = std::make_unique<image::Scanline>(spec, size.width);

    // copy the row pixels into the scanline
    for (std::size_t x = 0; x < size.width; x += 1) {
      // Ensure each input pixel has the right number of components.
      std::memcpy(scanline->dataAtPixel(x), pixel.data(), spec.bytesPerPixel);
    }

    result.push_back(std::move(scanline));
  }

  return result;
}

std::vector<std::unique_ptr<image::Scanline>> readJpegImage(
    const std::string& path) {
  io::FileImageSource imgSource{path};
  plugins::jpeg::LibJpegDecompressor decompressor{imgSource};
  const auto size = decompressor.outputImageSpecification().size;

  std::vector<std::unique_ptr<image::Scanline>> result;
  result.reserve(size.height);
  for (std::size_t y = 0; y < size.height; y += 1) {
    result.push_back(decompressor.readScanline());
  }
  return result;
}

} // namespace testutils
} // namespace comparison
} // namespace spectrum
} // namespace facebook
