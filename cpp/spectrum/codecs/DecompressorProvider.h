// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/codecs/IDecompressor.h>
#include <spectrum/image/Specification.h>
#include <spectrum/io/IImageSource.h>

#include <functional>
#include <vector>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace codecs {
/**
 * Defines the characteristics of a Decompressor in addition to providing
 * a factory to instantiate the Decompressor.
 */
struct DecompressorProvider {
  using Factory = std::function<std::unique_ptr<IDecompressor>(
      io::IImageSource& source,
      const folly::Optional<image::Ratio>& samplingRatio,
      const Configuration& configuration)>;

  image::Format format;
  std::vector<image::Ratio> supportedSamplingRatios;
  Factory decompressorFactory{nullptr};
};

} // namespace codecs
} // namespace spectrum
} // namespace facebook
