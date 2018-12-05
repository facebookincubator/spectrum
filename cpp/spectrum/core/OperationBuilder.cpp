// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "OperationBuilder.h"

namespace facebook {
namespace spectrum {
namespace core {

OperationBuilder::OperationBuilder(
    const Configuration& configuration,
    const codecs::Repository& codecsRepository,
    std::vector<codecs::EncodedImageFormatDetectorHandler>&&
        encodedImageFormatDetectorHandlers)
    : _configuration(configuration),
      _codecRepository(codecsRepository),
      _encodedImageSpecificationDetector(
          _codecRepository,
          _configuration,
          std::move(encodedImageFormatDetectorHandlers)) {}

Operation OperationBuilder::build(
    io::IBitmapImageSource& source,
    io::IImageSink& sink,
    const Options& options) const {
  return _build(source, sink, source.imageSpecification(), options);
}

Operation OperationBuilder::build(
    io::RewindableImageSource& source,
    io::IImageSink& sink,
    const Options& options) const {
  const auto inputImageSpecification =
      _encodedImageSpecificationDetector.detectImageSpecification(
          source, options);
  return _build(source, sink, inputImageSpecification, options);
}

Operation OperationBuilder::_build(
    io::IImageSource& source,
    io::IImageSink& sink,
    const image::Specification& inputImageSpecification,
    const Options& options) const {
  auto configuration = options.configuration;
  configuration.mergeInto(_configuration);

  return Operation{
      .io = _buildIO(source, sink),
      .codecs =
          _buildCodecs(inputImageSpecification.format, options.outputFormat()),
      .parameters = _buildParameters(options, inputImageSpecification),
      .configuration = configuration,
  };
}

Operation::IO OperationBuilder::_buildIO(
    io::IImageSource& source,
    io::IImageSink& sink) const {
  return Operation::IO{
      .source = source,
      .sink = sink,
  };
}

Operation::Codecs OperationBuilder::_buildCodecs(
    const image::Format& inputImageFormat,
    const image::Format& outputImageFormat) const {
  return Operation::Codecs{
      .decompressorProvider =
          _codecRepository.decompressorProvider(inputImageFormat),
      .compressorProvider =
          _codecRepository.compressorProvider(outputImageFormat),
  };
}

Operation::Parameters OperationBuilder::_buildParameters(
    const Options& options,
    const image::Specification& inputImageSpecification) const {
  return Operation::Parameters{
      .inputImageSpecification = inputImageSpecification,
      .outputImageFormat = options.outputFormat(),
      .transformations = options.transformations,
      .encodeRequirement = options.encodeRequirement,
      .extraMetadata = options.metadata,
      .outputPixelSpecificationRequirement =
          options.outputPixelSpecificationRequirement,
  };
}

} // namespace core
} // namespace spectrum
} // namespace facebook
