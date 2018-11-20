// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Scanline.h>
#include <spectrum/image/Specification.h>

#include <memory>
#include <vector>

#include <folly/Optional.h>
#include <folly/Range.h>

namespace facebook {
namespace spectrum {

namespace codecs {
namespace error {
extern const folly::StringPiece
    DecompressorUnsupportedPixelSpecificationOverride;
extern const folly::StringPiece DecompressorUnexpectedPixelSpecification;
extern const folly::StringPiece DecompressorUnexpectedChromaSamplingMode;
extern const folly::StringPiece DecompressorCannotReadPixelSpecification;
extern const folly::StringPiece DecompressorCannotSample;
extern const folly::StringPiece DecompressorFailure;
} // namespace error

/**
 * IDecompressor is an abstract class defining the capabilities of an image
 * decompressor.
 */
class IDecompressor {
 public:
  virtual ~IDecompressor() = default;

  /**
   * The specification of the image referenced by the image source.
   */
  virtual image::Specification sourceImageSpecification() = 0;

  /**
   * The specification of the image referenced by the image source after
   * modifications by the decompressor.
   */
  virtual image::Specification outputImageSpecification() = 0;

  /**
   * The next scanline from the image source.
   */
  virtual std::unique_ptr<image::Scanline> readScanline() = 0;

 protected:
  IDecompressor() = default;
  IDecompressor(const IDecompressor&) = delete;
  IDecompressor(IDecompressor&&) = default;

  /**
   * Throws an exception if a sampling ratio is specified. Decompressors not
   * supporting sampling ratios should call this function in their constructor.
   *
   * @param samplingRatio the sampling ratio to use.
   */
  static void _ensureNoSamplingRatio(
      const folly::Optional<image::Ratio>& samplingRatio);
};

} // namespace codecs
} // namespace spectrum
} // namespace facebook
