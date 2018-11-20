// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ICompressor.h"

namespace facebook {
namespace spectrum {
namespace codecs {
namespace error {
const folly::StringPiece CompressorCannotWritePixelSpecification{
    "compressor_unsupported_pixel_specification"};
const folly::StringPiece CompressorRequiresLossy{"compressor_requires_lossy"};
const folly::StringPiece CompressorCannotEncodeMetadata{
    "compressor_cannot_encode_metadata"};
const folly::StringPiece CompressorRequiresPixelSpecification{
    "compressor_requires_pixel_specification"};
const folly::StringPiece CompressorRequiresLossless{
    "compressor_requires_lossless"};
const folly::StringPiece CompressorFailure{"compressor_failure"};
const folly::StringPiece CompressorInputSizeTooLarge{
    "compressor_input_size_too_large"};
} // namespace error

void ICompressor::enforceLossless(
    const folly::Optional<requirements::Encode>& encodeRequirement) {
  SPECTRUM_ERROR_IF(
      encodeRequirement.hasValue() &&
          encodeRequirement->mode == requirements::Encode::Mode::Lossy,
      error::CompressorRequiresLossless);
}

void ICompressor::enforceLossy(
    const folly::Optional<requirements::Encode>& encodeRequirement) {
  SPECTRUM_ERROR_IF(
      encodeRequirement.hasValue() &&
          encodeRequirement->mode == requirements::Encode::Mode::Lossless,
      error::CompressorRequiresLossy);
}

void ICompressor::enforceCannotEncodeMetadata(
    const folly::Optional<image::Metadata>& metadata) {
  SPECTRUM_ERROR_IF(
      metadata.hasValue() && !metadata->empty(),
      error::CompressorCannotEncodeMetadata);
}

image::pixel::Specification ICompressor::enforcePixelSpecificationSet(
    const folly::Optional<image::pixel::Specification>& pixelSpecification) {
  SPECTRUM_ERROR_IF_NOT(
      pixelSpecification.hasValue(),
      error::CompressorRequiresPixelSpecification);
  return *pixelSpecification;
}

requirements::Encode::Quality ICompressor::_sanitizedQualityWithDefault(
    const folly::Optional<requirements::Encode>& encodeRequirement,
    const requirements::Encode::Quality defaultQuality,
    const requirements::Encode::Quality minQuality,
    const requirements::Encode::Quality maxQuality) {
  if (encodeRequirement.hasValue()) {
    return encodeRequirement->sanitizedQuality(
        defaultQuality, minQuality, maxQuality);
  } else {
    return defaultQuality;
  }
}

void ICompressor::enforceSizeBelowMaximumSideDimension(
    const image::Size& size,
    const std::uint32_t maximumSideDimensions) {
  SPECTRUM_ERROR_IF(
      size.width > maximumSideDimensions || size.height > maximumSideDimensions,
      error::CompressorInputSizeTooLarge);
}

} // namespace codecs
} // namespace spectrum
} // namespace facebook
