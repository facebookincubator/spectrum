// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Geometry.h>
#include <spectrum/image/Metadata.h>
#include <spectrum/image/Pixel.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/requirements/Encode.h>

#include <folly/Optional.h>
#include <folly/Range.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace error {
extern const folly::StringPiece CompressorCannotWritePixelSpecification;
extern const folly::StringPiece CompressorRequiresLossy;
extern const folly::StringPiece CompressorCannotEncodeMetadata;
extern const folly::StringPiece CompressorRequiresPixelSpecification;
extern const folly::StringPiece CompressorRequiresLossless;
extern const folly::StringPiece CompressorInputSizeTooLarge;
extern const folly::StringPiece CompressorFailure;
} // namespace error

/**
 * ICompressor is an abstract class defining the capabilities of an image
 * compressor.
 * @note Compressors should never convert scanlines on write and expect the
 * proper `writeScanline` method to be called depending on the value of
 * `getOutputPixelSpecification`.
 */
class ICompressor {
 public:
  virtual ~ICompressor() = default;

  /**
   * Writes the scanline in the image sink.
   * @note Overrides should throw if the compressor doesn't support writting the
   * pixel type of the scanline or if the scanline does not match the type
   * specified in the constructor.
   *
   * @param scanline The scanline to write.
   */
  virtual void writeScanline(std::unique_ptr<image::Scanline> scanline) = 0;

  /**
   * Throws an exception if there's no quality defined. Lossy compressors should
   * call this function in their constructors.
   *
   * @param encodeRequirement the compression requirement passed to the
   * compressor.
   */
  static void enforceMustCompress(
      const folly::Optional<requirements::Encode>& encodeRequirement);

  /**
   * Throws an exception if the encode requirement requires lossy encoding.
   * Lossy compressors should call this function in their constructors.
   *
   * @param encodeRequirement the encode requirement passed to the compressor.
   */
  static void enforceLossless(
      const folly::Optional<requirements::Encode>& encodeRequirement);

  /**
   * Throws an exception if the encode requirement requires a lossless encoding.
   * Lossless compressors should call this function in their constructors.
   *
   * @param encodeRequirement the encode requirement passed to the compressor.
   */
  static void enforceLossy(
      const folly::Optional<requirements::Encode>& encodeRequirement);

  /**
   * Throws an exception if metadata is passed. Compressors without metadata
   * support should call this function in their constructors.
   *
   * @param metadata the metadata passed to the compressor.
   */
  static void enforceCannotEncodeMetadata(
      const folly::Optional<image::Metadata>& metadata);

  /**
   * Throws an exception if the pixel specification isn't specified. Compressors
   * requiring an explicit pixel specification should call this function in
   * their constroctors.
   *
   * @param pixelSpecification The pixel specification to check.
   */
  static image::pixel::Specification enforcePixelSpecificationSet(
      const folly::Optional<image::pixel::Specification>& pixelSpecification);

  /**
   * Throws an exception if one of the size component is beyond the provided
   * maximum side dimension.
   *
   * @param size The image size.
   * @param maximumSideDimensions The maximum supported side dimension.
   */
  static void enforceSizeBelowMaximumSideDimension(
      const image::Size& size,
      const std::uint32_t maximumSideDimensions);

 protected:
  ICompressor() = default;
  ICompressor(const ICompressor&) = delete;
  ICompressor(ICompressor&&) = default;

  /**
   * Returns the sanitized quality from the encode requirement or the default
   * value.
   */
  static requirements::Encode::Quality _sanitizedQualityWithDefault(
      const folly::Optional<requirements::Encode>& encodeRequirement,
      const requirements::Encode::Quality defaultQuality =
          requirements::Encode::QualityDefault,
      const requirements::Encode::Quality minQuality =
          requirements::Encode::QualityMin,
      const requirements::Encode::Quality maxQuality =
          requirements::Encode::QualityMax);
};

} // namespace codecs
} // namespace spectrum
} // namespace facebook
