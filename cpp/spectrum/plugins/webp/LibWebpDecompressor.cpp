// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibWebpDecompressor.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/plugins/webp/LibWebpConstants.h>

#include "webp/decode.h"

#include <folly/Optional.h>

#include <array>
#include <csetjmp>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace webp {

namespace { /* anonymous */
WEBP_CSP_MODE pixelSpecificationToCspMode(
    const image::pixel::Specification pixelSpecification) {
  if (pixelSpecification == image::pixel::specifications::RGB) {
    return MODE_RGB;
  } else if (pixelSpecification == image::pixel::specifications::ARGB) {
    return MODE_ARGB;
  } else if (pixelSpecification == image::pixel::specifications::RGBA) {
    return MODE_RGBA;
  } else {
    SPECTRUM_ERROR_STRING(
        codecs::error::DecompressorUnexpectedPixelSpecification,
        pixelSpecification.string());
  }
}
} // namespace

LibWebpDecompressor::LibWebpDecompressor(
    io::IImageSource& source,
    const folly::Optional<image::Ratio>& samplingRatio,
    const folly::Optional<image::pixel::Specification>&
        overridePixelSpecification)
    : _source(source), _overridePixelSpecification(overridePixelSpecification) {
  IDecompressor::_ensureNoSamplingRatio(samplingRatio);
}

LibWebpDecompressor::~LibWebpDecompressor() {
  if (_webpDecoder != nullptr) {
    WebPIDelete(_webpDecoder);
  }
}

//
// Private
//

void LibWebpDecompressor::_ensureHeaderIsRead() {
  if (_isHeaderRead) {
    return; // only read header once
  }

  // The WebP header is quite compact and using the DefaultBufferSize would be
  // wasteful here
  constexpr std::size_t HEADER_BUFFER_SIZE = 32;

  auto status = VP8_STATUS_NOT_ENOUGH_DATA;
  while (status == VP8_STATUS_NOT_ENOUGH_DATA) {
    // read next chunk
    std::vector<char> chunk(HEADER_BUFFER_SIZE);
    const auto bytesRead = _source.read(chunk.data(), HEADER_BUFFER_SIZE);
    if (bytesRead == 0) {
      break;
    }

    // append to already collected header bytes
    std::copy_n(chunk.begin(), bytesRead, std::back_inserter(_webpPayload));

    // try to extract features
    status = WebPGetFeatures(
        reinterpret_cast<std::uint8_t*>(_webpPayload.data()),
        _webpPayload.size(),
        &_webpFeatures);
  }

  SPECTRUM_ERROR_CSTR_IF_NOT(
      status == VP8_STATUS_OK,
      codecs::error::DecompressorFailure,
      "webp_get_features_failed");

  SPECTRUM_ERROR_CSTR_IF(
      _webpFeatures.height > maximumSizeDimension ||
          _webpFeatures.width > maximumSizeDimension,
      codecs::error::DecompressorFailure,
      "webp_input_size_too_large");

  _isHeaderRead = true;
}

void LibWebpDecompressor::_ensureImageIsRead() {
  if (_isImageRead) {
    return;
  }

  _ensureHeaderIsRead();

  // init buffer
  const auto pixelSpecification = outputImageSpecification().pixelSpecification;
  const auto bytesPerPixel = pixelSpecification.bytesPerPixel;
  const auto stride = _webpFeatures.width * bytesPerPixel;

  _rawImage.resize(_webpFeatures.height * stride);

  // init config
  WebPDecoderConfig config;
  const auto webpConfigInitSuccess = WebPInitDecoderConfig(&config);

  SPECTRUM_ERROR_CSTR_IF_NOT(
      webpConfigInitSuccess,
      codecs::error::DecompressorFailure,
      "webp_init_decoder_config_failed");

  // init decoder
  _webpDecoder = WebPINewDecoder(&config.output);

  SPECTRUM_ERROR_CSTR_IF(
      _webpDecoder == nullptr,
      codecs::error::DecompressorFailure,
      "webp_i_new_decoder_failed");

  // collect rest of input
  std::size_t bytesRead;
  do {
    std::array<char, core::DefaultBufferSize> chunk{};
    bytesRead = _source.read(chunk.data(), core::DefaultBufferSize);
    std::copy_n(chunk.begin(), bytesRead, std::back_inserter(_webpPayload));
  } while (bytesRead > 0);

  // read image information
  const auto getFeaturesStatus = WebPGetFeatures(
      reinterpret_cast<std::uint8_t*>(_webpPayload.data()),
      _webpPayload.size(),
      &config.input);

  SPECTRUM_ERROR_CSTR_IF_NOT(
      getFeaturesStatus == VP8_STATUS_OK,
      codecs::error::DecompressorFailure,
      "webp_get_features_failed");

  // set options
  config.options.no_fancy_upsampling = 1;
  config.options.use_threads = 0;

  config.output.colorspace = pixelSpecificationToCspMode(pixelSpecification);
  config.output.is_external_memory = 1;
  config.output.u.RGBA.rgba = _rawImage.data();
  config.output.u.RGBA.size = _rawImage.size();
  config.output.u.RGBA.stride = stride;

  // decode image
  const auto decodeStatus = WebPDecode(
      reinterpret_cast<std::uint8_t*>(_webpPayload.data()),
      _webpPayload.size(),
      &config);
  _webpPayload.clear(); // payload bytes are no longer needed

  SPECTRUM_ERROR_CSTR_IF_NOT(
      decodeStatus == VP8_STATUS_OK,
      codecs::error::DecompressorFailure,
      "webp_decode_failed");

  _isImageRead = true;
}

std::unique_ptr<image::Scanline> LibWebpDecompressor::readScanline() {
  _ensureImageIsRead();
  SPECTRUM_ENFORCE_IF_NOT(_outputScanline < _webpFeatures.height);

  const auto pixelSpecification = outputImageSpecification().pixelSpecification;

  const auto width = _webpFeatures.width;
  auto scanline = std::make_unique<image::Scanline>(pixelSpecification, width);

  const std::size_t bytesPerPixel = pixelSpecification.bytesPerPixel;
  const auto stride = width * bytesPerPixel;
  const auto offset = _outputScanline * stride;

  SPECTRUM_ENFORCE_IF_NOT(offset + stride <= _rawImage.size());

  std::memcpy(scanline->data(), _rawImage.data() + offset, stride);

  ++_outputScanline;

  // free memory early
  if (_outputScanline == _webpFeatures.height) {
    _rawImage.clear();
  }

  return scanline;
}

image::Specification LibWebpDecompressor::sourceImageSpecification() {
  if (_sourceImageSpecification.hasValue()) {
    return *_sourceImageSpecification;
  }

  _ensureHeaderIsRead();

  const auto pixelSpecification = _webpFeatures.has_alpha
      ? image::pixel::specifications::ARGB
      : image::pixel::specifications::RGB;

  _sourceImageSpecification = image::Specification{
      .size =
          image::Size{
              SPECTRUM_CONVERT_OR_THROW(_webpFeatures.width, std::uint32_t),
              SPECTRUM_CONVERT_OR_THROW(_webpFeatures.height, std::uint32_t),
          },
      .format = image::formats::Webp,
      .pixelSpecification = pixelSpecification,
  };

  return *_sourceImageSpecification;
}

image::Specification LibWebpDecompressor::outputImageSpecification() {
  auto outputImageSpecification = sourceImageSpecification();

  if (_overridePixelSpecification.hasValue()) {
    outputImageSpecification.pixelSpecification = *_overridePixelSpecification;
  }

  return outputImageSpecification;
}

} // namespace webp
} // namespace plugins
} // namespace spectrum
} // namespace facebook
