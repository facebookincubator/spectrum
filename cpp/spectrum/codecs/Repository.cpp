// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Repository.h"

#include <spectrum/Configuration.h>
#include <spectrum/codecs/ICompressor.h>
#include <spectrum/codecs/IDecompressor.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/io/IImageSource.h>

#include <exception>
#include <utility>
#include <vector>

namespace facebook {
namespace spectrum {
namespace codecs {

namespace errors {
const folly::StringPiece CodecRepositoryDecompressorNotFound{
    "codec_repo_decompressor_not_found"};
const folly::StringPiece CodecRepositoryCompressorNotFound{
    "codec_repo_compressor_not_found"};
} // namespace errors

Repository::Repository(
    std::vector<DecompressorProvider>&& decompressorProviders,
    std::vector<CompressorProvider>&& compressorProviders)
    : _compressorProviders(std::move(compressorProviders)),
      _decompressorProviders(std::move(decompressorProviders)) {}

void Repository::addCompressorProvider(
    const CompressorProvider& compressorProvider) {
  _compressorProviders.push_back(compressorProvider);
}

void Repository::addDecompressorProvider(
    const DecompressorProvider& decompressorProvider) {
  _decompressorProviders.push_back(decompressorProvider);
}

CompressorProvider Repository::compressorProvider(
    const image::Format& imageFormat) const {
  for (const auto& candidate : _compressorProviders) {
    if (candidate.format == imageFormat) {
      return candidate;
    }
  }

  SPECTRUM_ERROR_STRING(
      errors::CodecRepositoryCompressorNotFound,
      imageFormat.identifier().toStdString());
}

DecompressorProvider Repository::decompressorProvider(
    const image::Format& imageFormat) const {
  for (const auto& candidate : _decompressorProviders) {
    if (candidate.format == imageFormat) {
      return candidate;
    }
  }

  SPECTRUM_ERROR_STRING(
      errors::CodecRepositoryDecompressorNotFound,
      imageFormat.identifier().toStdString());
}

} // namespace codecs
} // namespace spectrum
} // namespace facebook
