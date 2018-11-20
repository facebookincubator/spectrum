// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "EncodedImageSpecificationDetector.h"

#include <spectrum/io/RewindableImageSource.h>

#include <folly/Range.h>

#include <cstring>

namespace facebook {
namespace spectrum {
namespace codecs {

EncodedImageSpecificationDetector::EncodedImageSpecificationDetector(
    const Repository& codecRepository,
    const Configuration& configuration,
    std::vector<EncodedImageFormatDetectorHandler>&&
        encodedImageFormatDetectorHandlers)
    : _codecRepository(codecRepository),
      _configuration(configuration),
      _encodedImageFormatDetector(
          std::move(encodedImageFormatDetectorHandlers)) {}

image::Specification
EncodedImageSpecificationDetector::detectImageSpecification(
    io::RewindableImageSource& source,
    const Options& options) const {
  const auto detectedFormat = _encodedImageFormatDetector.detectFormat(source);

  const auto decompressorProvider =
      _codecRepository.decompressorProvider(detectedFormat);

  auto configuration = _configuration;
  configuration.merge(options.configuration);

  source.mark();

  auto decompressor = decompressorProvider.decompressorFactory(
      source, folly::none, configuration);

  const auto imageSpecification = decompressor->outputImageSpecification();

  source.reset();

  return imageSpecification;
}

} // namespace codecs
} // namespace spectrum
} // namespace facebook
