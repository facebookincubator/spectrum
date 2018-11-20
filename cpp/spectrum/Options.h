// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/Transformations.h>
#include <spectrum/image/Specification.h>
#include <spectrum/requirements/Crop.h>
#include <spectrum/requirements/Encode.h>
#include <spectrum/requirements/Resize.h>
#include <spectrum/requirements/Rotate.h>

#include <atomic>
#include <string>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {

/**
 * The Options object encapsulates all parameters for an operation. In most
 * cases subclasses (DecodeOptions, TranscodeOptions, EncodeOptions,
 * TransformOptions) should be used instead of this type directly.
 */
struct Options {
  Options() = delete;

  /**
   * Deprecated API - will be removed.
   * TODO T27151229: Make protected once Android and iOS have the new
   * decode/encode/transcode abstractions
   */
  Options(
      const folly::Optional<requirements::Encode>& encodeRequirement,
      const Transformations& transformations,
      const folly::Optional<image::Metadata>& metadata,
      const Configuration& configuration,
      const folly::Optional<image::pixel::Specification>&
          outputPixelSpecificationRequirement);

  /**
   * The encode requirement to apply. Dictates the final image format. If none,
   * bitmap is deduced.
   */
  const folly::Optional<requirements::Encode> encodeRequirement;

  /**
   * Contains optional transformations such as resize, cropping and rotation.
   */
  const Transformations transformations;

  /**
   * Optional metadata to consider besiders the ones provided by the encoded
   * image.
   */
  const folly::Optional<image::Metadata> metadata;

  /**
   * If set, defines the pixel specification images should be converted to
   * before being passed to the compressor. An exception will be thrown if the
   * pixel specification doesn't fit the image format.
   */
  const folly::Optional<image::pixel::Specification>
      outputPixelSpecificationRequirement;

  /**
   * Specified options in the configuration object will override the default
   * configuration.
   */
  const Configuration configuration;

  /**
   * The option's output format. By default Bitmap will be returned unless an
   * encode requirement is set.
   */
  image::Format outputFormat() const;

  bool operator==(const Options& rhs) const;
};

/**
 * This Options object encapsulates all parameters for a decode operation. A
 * decode operation reads an encoded image and creates a bitmap.
 */
struct DecodeOptions : public Options {
 public:
  DecodeOptions(
      const Transformations& transformations = {},
      const Configuration& configuration = Configuration(),
      const folly::Optional<image::pixel::Specification>&
          outputPixelSpecificationRequirement = folly::none);
};

/**
 * This Options object encapsulates all parameters for an encode operation. An
 * encode operation reads a bitmap and creates an encoded image.
 */
struct EncodeOptions : public Options {
 public:
  EncodeOptions(
      const requirements::Encode& encodeRequirement,
      const Transformations& transformations = {},
      const folly::Optional<image::Metadata>& metadata = folly::none,
      const Configuration& configuration = Configuration(),
      const folly::Optional<image::pixel::Specification>&
          outputPixelSpecificationRequirement = folly::none);
};

/**
 * This Options object encapsulates all parameters for a transcode operation. A
 * transcode operation reads an encoded and creates an encoded image.
 */
struct TranscodeOptions : public Options {
 public:
  TranscodeOptions(
      const requirements::Encode& encodeRequirement,
      const Transformations& transformations = {},
      const folly::Optional<image::Metadata>& metadata = folly::none,
      const Configuration& configuration = Configuration(),
      const folly::Optional<image::pixel::Specification>&
          outputPixelSpecificationRequirement = folly::none);
};

/**
 * This Options object encapsulates all parameters for a transform operation. A
 * transcode operation reads a bitmap and creates a bitmap.
 */
struct TransformOptions : public Options {
 public:
  TransformOptions(
      const Transformations& transformations = {},
      const Configuration& configuration = Configuration(),
      const folly::Optional<image::pixel::Specification>&
          outputPixelSpecificationRequirement = folly::none);
};

} // namespace spectrum
} // namespace facebook
