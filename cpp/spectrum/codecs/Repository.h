// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/codecs/CompressorProvider.h>
#include <spectrum/codecs/DecompressorProvider.h>

#include <vector>

#include <folly/Range.h>

namespace facebook {
namespace spectrum {
namespace codecs {

namespace errors {
extern const folly::StringPiece CodecRepositoryDecompressorNotFound;
extern const folly::StringPiece CodecRepositoryCompressorNotFound;
} // namespace errors

/**
 * Single point for retrieving factories for compressors/decompressors for
 * various codecs. The factory methods are loaded into here by the plugins.
 */
class Repository {
 public:
  Repository(
      std::vector<DecompressorProvider>&& decompressorProviders = {},
      std::vector<CompressorProvider>&& compressorProviders = {});

  void addCompressorProvider(const CompressorProvider& compressorProvider);
  void addDecompressorProvider(
      const DecompressorProvider& decompressorProvider);

  CompressorProvider compressorProvider(const image::Format& imageFormat) const;
  DecompressorProvider decompressorProvider(
      const image::Format& imageFormat) const;

 private:
  std::vector<CompressorProvider> _compressorProviders;
  std::vector<DecompressorProvider> _decompressorProviders;
};

} // namespace codecs
} // namespace spectrum
} // namespace facebook
