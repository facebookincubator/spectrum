// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/IDecompressor.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace testutils {

class FakeDecompressor : public IDecompressor {
 private:
  image::Size _size;
  image::pixel::Specification _pixelSpecification;

 public:
  FakeDecompressor(
      const image::Size& size,
      const image::pixel::Specification& pixelSpecification);

  image::Specification sourceImageSpecification() override;
  image::Specification outputImageSpecification() override;

  std::unique_ptr<image::Scanline> readScanline() override;
};

} // namespace testutils
} // namespace codecs
} // namespace spectrum
} // namespace facebook
