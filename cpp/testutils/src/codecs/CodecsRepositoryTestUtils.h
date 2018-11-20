// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/ICompressor.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/image/Geometry.h>
#include <spectrum/image/Pixel.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace testutils {

Repository makeCodecRepositoryWithFakeDecompressor(
    const image::Size& decompressorImageSize = {2, 2},
    const image::pixel::Specification& decompressorPixelSpecification =
        image::pixel::specifications::Gray);
}
} // namespace codecs
} // namespace spectrum
} // namespace facebook
