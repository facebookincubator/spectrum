// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/Options.h>
#include <spectrum/Plugin.h>
#include <spectrum/Result.h>
#include <spectrum/Rule.h>
#include <spectrum/codecs/EncodedImageSpecificationDetector.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/core/OperationBuilder.h>
#include <spectrum/core/PluginAggregator.h>
#include <spectrum/core/RuleMatcher.h>
#include <spectrum/io/IBitmapImageSink.h>
#include <spectrum/io/IBitmapImageSource.h>
#include <spectrum/io/IEncodedImageSink.h>
#include <spectrum/io/IEncodedImageSource.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/io/IImageSource.h>

#include <folly/Range.h>

#include <chrono>
#include <initializer_list>
#include <memory>
#include <utility>
#include <vector>

namespace facebook {
namespace spectrum {

namespace error {
extern const folly::StringPiece EmptyInputSource;
} // namespace error

/**
 * Main entry point for Spectrum. Once created, a Spectrum instance is thread
 * safe. While creating a new instance with plugins is a lightweight operation,
 * it is recommended to keep a Spectrum instance around if possible.
 */
class Spectrum {
 public:
  /**
   * Creates Spectrum with the specified plugins / configuration.
   *
   * @param plugins The plugins to load.
   * @param configuration The base configuration to use.
   */
  Spectrum(
      std::vector<Plugin>&& plugins = {},
      const Configuration& configuration = Configuration())
      : Spectrum(core::PluginAggregator(std::move(plugins)), configuration) {}

  /**
   * Decodes the image originating from the source into the sink using
   * options.
   *
   * @param source The source from which the original image will be read from.
   * @param sink The sink in which the resulting image will be written to.
   * @param options The options describing the operation (optional).
   * @return Result object containing information about the executed decode.
   */
  Result decode(
      io::IEncodedImageSource& source,
      io::IBitmapImageSink& sink,
      const DecodeOptions& options = DecodeOptions()) const;

  /**
   * Encodes the image originating from the source into the sink using
   * options.
   *
   * @param source The source from which the original image will be read from.
   * @param sink The sink in which the resulting image will be written to.
   * @param options The options describing the operation.
   * @return Result object containing information about the executed encode.
   */
  Result encode(
      io::IBitmapImageSource& source,
      io::IEncodedImageSink& sink,
      const EncodeOptions& options) const;

  /**
   * Transcodes the image originating from the source into the sink using
   * options.
   *
   * @param source The source from which the original image will be read from.
   * @param sink The sink in which the resulting image will be written to.
   * @param options The options describing the operation.
   * @return Result object containing information about the executed transcode.
   */
  Result transcode(
      io::IEncodedImageSource& source,
      io::IEncodedImageSink& sink,
      const TranscodeOptions& options) const;

  /**
   * Transforms the image originating from the source into the sink using
   * options.
   *
   * @param source The source from which the original image will be read from.
   * @param sink The sink in which the resulting image will be written to.
   * @param options The options describing the transcoding.
   * @return Result object containing information about the executed
   * transformation.
   */
  Result transform(
      io::IBitmapImageSource& source,
      io::IBitmapImageSink& sink,
      const TransformOptions& options = TransformOptions()) const;

 private:
  Configuration _configuration;
  codecs::Repository _codecRepository;
  core::RuleMatcher _ruleMatcher;
  core::OperationBuilder _operationBuilder;

  Spectrum(
      core::PluginAggregator&& pluginAggregator,
      const Configuration& configuration);

  Result _run(
      const core::Operation& operation,
      const std::chrono::high_resolution_clock::time_point startTime) const;

  Result _runEncoded(
      io::IEncodedImageSource& source,
      io::IImageSink& sink,
      const Options& options,
      const std::chrono::high_resolution_clock::time_point startTime) const;
};

} // namespace spectrum
} // namespace facebook
