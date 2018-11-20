// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Operation.h"

namespace facebook {
namespace spectrum {
namespace core {

std::unique_ptr<codecs::IDecompressor> Operation::makeDecompressor(
    const folly::Optional<image::Ratio>& samplingRatio) const {
  return codecs.decompressorProvider.decompressorFactory(
      io.source,
      samplingRatio,
      configuration);
}

std::unique_ptr<codecs::ICompressor> Operation::makeCompressor(
    const image::Specification& outputImageSpecification) const {
  const auto options = codecs::CompressorOptions{
      .sink = io.sink,
      .imageSpecification = outputImageSpecification,
      .encodeRequirement = parameters.encodeRequirement,
      .configuration = configuration,
  };

  return codecs.compressorProvider.compressorFactory(options);
}

} // namespace core
} // namespace spectrum
} // namespace facebook
