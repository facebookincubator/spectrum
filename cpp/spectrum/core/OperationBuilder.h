// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/Options.h>
#include <spectrum/codecs/EncodedImageFormatDetectorHandlers.h>
#include <spectrum/codecs/EncodedImageSpecificationDetector.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/core/Operation.h>
#include <spectrum/io/IBitmapImageSource.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/io/RewindableImageSource.h>

namespace facebook {
namespace spectrum {
namespace core {

class OperationBuilder {
 public:
  OperationBuilder(
      const Configuration& configuration,
      const codecs::Repository& codecsRepository,
      std::vector<codecs::EncodedImageFormatDetectorHandler>&&
          encodedImageFormatDetectorHandlers);

  Operation build(
      io::IBitmapImageSource& source,
      io::IImageSink& sink,
      const Options& options) const;

  Operation build(
      io::RewindableImageSource& source,
      io::IImageSink& sink,
      const Options& options) const;

 private:
  const Configuration& _configuration;
  const codecs::Repository& _codecRepository;
  const codecs::EncodedImageSpecificationDetector
      _encodedImageSpecificationDetector;
  Operation _build(

      io::IImageSource& source,
      io::IImageSink& sink,
      const image::Specification& inputImageSpecification,
      const Options& options) const;

  Operation::Parameters _buildParameters(
      const Options& options,
      const image::Specification& inputImageSpecification) const;
  Operation::IO _buildIO(io::IImageSource& source, io::IImageSink& sink) const;
  Operation::Codecs _buildCodecs(
      const image::Format& inputImageFormat,
      const image::Format& outputImageFormat) const;
};

} // namespace core
} // namespace spectrum
} // namespace facebook
