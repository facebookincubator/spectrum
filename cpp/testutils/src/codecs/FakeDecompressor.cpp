// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "FakeDecompressor.h"

namespace facebook {
namespace spectrum {
namespace codecs {
namespace testutils {

FakeDecompressor::FakeDecompressor(
    const image::Size& size,
    const image::pixel::Specification& pixelSpecification)
    : _size(size), _pixelSpecification(pixelSpecification) {}

image::Specification FakeDecompressor::sourceImageSpecification() {
  return image::Specification{
      .size = _size,
      .format = image::formats::Jpeg,
      .pixelSpecification = _pixelSpecification,
  };
}

image::Specification FakeDecompressor::outputImageSpecification() {
  return sourceImageSpecification();
}

std::unique_ptr<image::Scanline> FakeDecompressor::readScanline() {
  return nullptr;
}

} // namespace testutils
} // namespace codecs
} // namespace spectrum
} // namespace facebook
