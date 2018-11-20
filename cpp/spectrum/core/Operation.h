// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/Transformations.h>
#include <spectrum/codecs/CompressorProvider.h>
#include <spectrum/codecs/DecompressorProvider.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/image/Metadata.h>
#include <spectrum/image/Specification.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/requirements/Encode.h>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace core {

struct Operation {
  struct IO {
    io::IImageSource& source;
    io::IImageSink& sink;
  };

  struct Codecs {
    codecs::DecompressorProvider decompressorProvider;
    codecs::CompressorProvider compressorProvider;
  };

  struct Parameters {
    image::Specification inputImageSpecification;
    image::Format outputImageFormat;
    Transformations transformations;
    folly::Optional<requirements::Encode> encodeRequirement;
    folly::Optional<image::Metadata> extraMetadata;
    folly::Optional<image::pixel::Specification>
        outputPixelSpecificationRequirement;
  };

  IO io;
  Codecs codecs;
  Parameters parameters;
  Configuration configuration;

  std::unique_ptr<codecs::IDecompressor> makeDecompressor(
      const folly::Optional<image::Ratio>& samplingRatio) const;
  std::unique_ptr<codecs::ICompressor> makeCompressor(
      const image::Specification& outputImageSpecification) const;
};

} // namespace core
} // namespace spectrum
} // namespace facebook
