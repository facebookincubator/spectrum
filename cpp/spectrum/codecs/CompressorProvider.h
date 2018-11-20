// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/codecs/ICompressor.h>
#include <spectrum/image/Metadata.h>
#include <spectrum/image/Specification.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/requirements/Encode.h>

#include <folly/Optional.h>

#include <functional>

namespace facebook {
namespace spectrum {
namespace codecs {

struct CompressorOptions {
  io::IImageSink& sink;
  image::Specification imageSpecification;
  folly::Optional<requirements::Encode> encodeRequirement;
  Configuration configuration;
};

/**
 * Defines the characteristics of a Compressor in addition to providing
 * a factory to instantiate the Compressor.
 */
struct CompressorProvider {
  using Factory = std::function<std::unique_ptr<ICompressor>(
      const CompressorOptions& options)>;

  using PixelSpecificationNarrower = std::function<image::pixel::Specification(
      const image::pixel::Specification& pixelSpecification)>;

  image::Format format;
  bool supportsSettingMetadata{false};

  /**
   * Narrows the output pixel specification to a pixel specification the
   * compressor accepts as an input. `nullptr` allows any pixel specification.
   */
  PixelSpecificationNarrower pixelSpecificationNarrower{nullptr};

  /*
   * Supported chroma sampling mode by the compressor.
   */
  std::vector<image::ChromaSamplingMode> supportedChromaSamplingModes{};

  Factory compressorFactory{nullptr};
};

} // namespace codecs
} // namespace spectrum
} // namespace facebook
