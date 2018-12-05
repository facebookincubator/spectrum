// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Spectrum.h"

#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/RewindableImageSource.h>

#include <memory>
#include <vector>

namespace facebook {
namespace spectrum {
namespace error {
const folly::StringPiece EmptyInputSource{"empty_input_source"};
} // namespace error

namespace {
std::uint32_t _totalTime(
    const std::chrono::high_resolution_clock::time_point startTime) {
  const auto endTime = std::chrono::high_resolution_clock::now();
  const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      endTime - startTime);
  return SPECTRUM_CONVERT_OR_THROW(duration.count(), std::uint32_t);
}
} // namespace

Spectrum::Spectrum(
    core::PluginAggregator&& pluginAggregator,
    const Configuration& configuration)
    : _configuration(configuration),
      _codecRepository(
          std::move(pluginAggregator.decompressorProviders),
          std::move(pluginAggregator.compressorProviders)),
      _ruleMatcher(std::move(pluginAggregator.rules)),
      _operationBuilder(
          _configuration,
          _codecRepository,
          std::move(pluginAggregator.encodedImageFormatDetectorHandlers)) {}

Result Spectrum::decode(
    io::IEncodedImageSource& source,
    io::IBitmapImageSink& sink,
    const DecodeOptions& options) const {
  const auto startTime = std::chrono::high_resolution_clock::now();
  return _runEncoded(source, sink, options, startTime);
}

Result Spectrum::encode(
    io::IBitmapImageSource& source,
    io::IEncodedImageSink& sink,
    const EncodeOptions& options) const {
  const auto startTime = std::chrono::high_resolution_clock::now();
  return _run(_operationBuilder.build(source, sink, options), startTime);
}

Result Spectrum::transcode(
    io::IEncodedImageSource& source,
    io::IEncodedImageSink& sink,
    const TranscodeOptions& options) const {
  const auto startTime = std::chrono::high_resolution_clock::now();
  return _runEncoded(source, sink, options, startTime);
}

Result Spectrum::transform(
    io::IBitmapImageSource& source,
    io::IBitmapImageSink& sink,
    const TransformOptions& options) const {
  const auto startTime = std::chrono::high_resolution_clock::now();
  return _run(_operationBuilder.build(source, sink, options), startTime);
}

Result Spectrum::_runEncoded(
    io::IEncodedImageSource& source,
    io::IImageSink& sink,
    const Options& options,
    const std::chrono::high_resolution_clock::time_point startTime) const {
  auto rewindableImageSource = io::RewindableImageSource{source};
  SPECTRUM_ERROR_IF(
      rewindableImageSource.available() < 1, error::EmptyInputSource);
  return _run(
      _operationBuilder.build(rewindableImageSource, sink, options), startTime);
}

Result Spectrum::_run(
    const core::Operation& operation,
    const std::chrono::high_resolution_clock::time_point startTime) const {
  const auto rule = _ruleMatcher.findFirstMatching(operation.parameters);
  const auto outputImageSpecification =
      rule.recipeFactory()->perform(operation);

  return {
      .ruleName = rule.name,
      .inputImageSpecification = operation.parameters.inputImageSpecification,
      .outputImageSpecification = outputImageSpecification,
      .totalBytesRead = operation.io.source.getTotalBytesRead(),
      .totalBytesWritten = operation.io.sink.totalBytesWritten(),
      .duration = _totalTime(startTime),
  };
}

} // namespace spectrum
} // namespace facebook
