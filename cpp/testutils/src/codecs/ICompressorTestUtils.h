// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/codecs/ICompressor.h>
#include <spectrum/image/Specification.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/requirements/Encode.h>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace testutils {

struct CompressorOptions {
  io::IImageSink& sink;
  folly::Optional<image::Size> imageSize;
  folly::Optional<image::pixel::Specification> pixelSpecification;
  folly::Optional<requirements::Encode> encodeRequirement;
  folly::Optional<image::Metadata> metadata;
  Configuration configuration;
};

void writeRgbData(ICompressor& compressor, const image::Size& imageSize);

} // namespace testutils
} // namespace codecs
} // namespace spectrum
} // namespace facebook
