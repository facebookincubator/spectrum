// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Options.h>
#include <spectrum/codecs/EncodedImageFormatDetector.h>
#include <spectrum/codecs/Repository.h>
#include <spectrum/io/RewindableImageSource.h>

#include <functional>
#include <vector>

namespace facebook {
namespace spectrum {

namespace io {
class RewindableImageSource;
}

namespace codecs {

/**
 * Responsible for detecting the image information of an encoded image source.
 */
class EncodedImageSpecificationDetector {
 public:
  /**
   * Creates a new EncodedImageSpecificationDetector.
   *
   * @param codecRepository The codec repository containing all compressor /
   * decompressor providers.
   * @param configuration The base configuration.
   * @param encodedImageFormatDetectorHandlers The image format handlers to pass
   * down to `EncodedImageFormatDetector`.
   */
  EncodedImageSpecificationDetector(
      const Repository& codecRepository,
      const Configuration& configuration,
      std::vector<EncodedImageFormatDetectorHandler>&&
          encodedImageFormatDetectorHandlers);

  /**
   * Detects the input image information of an encoded image if not already
   * provided by the given options.
   *
   * @param source The image source to detect the image information from.
   * @param options The options of the current operation. Options will get
   * updated with the newly discovered information.
   * @return True if all image information were detected. False otherwise.
   */
  image::Specification detectImageSpecification(
      io::RewindableImageSource& source,
      const Options& options) const;

 private:
  const Repository& _codecRepository;
  const Configuration& _configuration;
  const EncodedImageFormatDetector _encodedImageFormatDetector;
};

} // namespace codecs
} // namespace spectrum
} // namespace facebook
