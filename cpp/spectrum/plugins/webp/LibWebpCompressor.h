// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>
#include <spectrum/codecs/CompressorProvider.h>
#include <spectrum/codecs/ICompressor.h>
#include <spectrum/io/IImageSink.h>

#include <folly/Optional.h>

#include <webp/encode.h>

#include <array>
#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace webp {

class LibWebpCompressor final : public codecs::ICompressor {
 private:
  struct WebP {
    WebPConfig configuration;
    WebPPicture picture;
  };

  const codecs::CompressorOptions _options;
  const requirements::Encode::Quality _quality;

  std::vector<std::uint8_t> _pixels;

  WebP _webp;
  std::size_t _currentScanline{0};
  bool _wasHeaderWritten{false};

  void _initialiseConfiguration();
  void _initialisePicture();
  void _ensureHeaderWritten();
  void _encodeIfFinished();

  static int _writeHandler(
      const std::uint8_t* data,
      std::size_t dataSize,
      const WebPPicture* picture);

 public:
  explicit LibWebpCompressor(const codecs::CompressorOptions& options);

  LibWebpCompressor(LibWebpCompressor&&) = default;

  ~LibWebpCompressor() override;

  //
  // Compressor
  //
 public:
  void writeScanline(std::unique_ptr<image::Scanline> scanline) override;
};
} // namespace webp
} // namespace plugins
} // namespace spectrum
} // namespace facebook
