// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "IDecompressor.h"

namespace facebook {
namespace spectrum {

namespace codecs {
namespace error {
const folly::StringPiece DecompressorUnsupportedPixelSpecificationOverride{
    "decompressor_unsupported_pixel_specification_override"};
const folly::StringPiece DecompressorUnexpectedPixelSpecification{
    "decompressor_unexpected_pixel_specification"};
const folly::StringPiece DecompressorCannotReadPixelSpecification{
    "decompressor_cannot_read_pixel_specification"};
const folly::StringPiece DecompressorUnexpectedChromaSamplingMode{
    "decompressor_unexpected_chroma_sampling"};
const folly::StringPiece DecompressorCannotSample{"decompressor_cannot_sample"};
const folly::StringPiece DecompressorFailure{"decompressor_failure"};
} // namespace error

void IDecompressor::_ensureNoSamplingRatio(
    const folly::Optional<image::Ratio>& samplingRatio) {
  SPECTRUM_ERROR_IF(
      samplingRatio.hasValue(), codecs::error::DecompressorCannotSample);
}

} // namespace codecs
} // namespace spectrum
} // namespace facebook
