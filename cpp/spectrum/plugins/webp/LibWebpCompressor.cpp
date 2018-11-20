// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibWebpCompressor.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/core/proc/ScanlineConversion.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/plugins/webp/LibWebpConstants.h>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace webp {

namespace {

WebPImageHint convertToWebPImageHint(
    const Configuration::Webp::ImageHint imageHint) {
  switch (imageHint) {
    case Configuration::Webp::ImageHint::Default:
      return WEBP_HINT_DEFAULT;

    case Configuration::Webp::ImageHint::Picture:
      return WEBP_HINT_PICTURE;

    case Configuration::Webp::ImageHint::Photo:
      return WEBP_HINT_PHOTO;

    case Configuration::Webp::ImageHint::Graph:
      return WEBP_HINT_GRAPH;

    default:
      SPECTRUM_UNREACHABLE_CONFIGURATION_WEBP_IMAGE_HINT(imageHint);
  }
}
} // namespace

LibWebpCompressor::LibWebpCompressor(const codecs::CompressorOptions& options)
    : _options(options),
      _quality(ICompressor::_sanitizedQualityWithDefault(
          options.encodeRequirement)) {
  ICompressor::enforceCannotEncodeMetadata(options.imageSpecification.metadata);
  ICompressor::enforceSizeBelowMaximumSideDimension(
      options.imageSpecification.size, maximumSizeDimension);

  // This is less than ideal, but I don't believe there's a straightforward way
  // to incrementally encode with webp. Will review this at a later stage.
  _pixels.reserve(
      options.imageSpecification.size.width *
      options.imageSpecification.size.height);
  _initialiseConfiguration();
  _initialisePicture();
}

LibWebpCompressor::~LibWebpCompressor() {
  WebPPictureFree(&_webp.picture);
}

void LibWebpCompressor::writeScanline(
    std::unique_ptr<image::Scanline> scanline) {
  const auto pixelSpecification = scanline->specification();
  SPECTRUM_ERROR_STRING_IF_NOT(
      pixelSpecification == image::pixel::specifications::RGBA,
      codecs::error::CompressorCannotWritePixelSpecification,
      pixelSpecification.string());

  _ensureHeaderWritten();

  _pixels.insert(
      _pixels.cend(),
      scanline->data(),
      scanline->data() + scanline->sizeBytes());

  scanline = nullptr;

  ++_currentScanline;

  _encodeIfFinished();
}

void LibWebpCompressor::_initialiseConfiguration() {
  const auto didInitializeConfig = WebPConfigInit(&_webp.configuration);

  SPECTRUM_ERROR_CSTR_IF_NOT(
      didInitializeConfig,
      codecs::error::CompressorFailure,
      "webp_config_init_failed");

  _webp.configuration.method = _options.configuration.webp.method();
  _webp.configuration.image_hint =
      convertToWebPImageHint(_options.configuration.webp.imageHint());

  const auto didValidateConfig = WebPValidateConfig(&_webp.configuration);

  SPECTRUM_ERROR_CSTR_IF_NOT(
      didValidateConfig,
      codecs::error::CompressorFailure,
      "webp_validate_config_failed");
}

void LibWebpCompressor::_initialisePicture() {
  const auto didInitializePicture = WebPPictureInit(&_webp.picture);

  SPECTRUM_ERROR_CSTR_IF_NOT(
      didInitializePicture,
      codecs::error::CompressorFailure,
      "webp_picture_init_failed");

  _webp.picture.use_argb = 1;
  _webp.picture.width = _options.imageSpecification.size.width;
  _webp.picture.height = _options.imageSpecification.size.height;
  _webp.picture.colorspace = WEBP_YUV420A;
  _webp.picture.writer = &_writeHandler;
  _webp.picture.custom_ptr = &_options.sink;
}

int LibWebpCompressor::_writeHandler(
    const std::uint8_t* data,
    std::size_t dataSize,
    const WebPPicture* picture) {
  io::IImageSink* const sink =
      reinterpret_cast<io::IImageSink*>(picture->custom_ptr);
  sink->write(reinterpret_cast<const char*>(data), dataSize);
  return 1;
}

void LibWebpCompressor::_encodeIfFinished() {
  if (_currentScanline < _options.imageSpecification.size.height) {
    return;
  }

  const auto didImportPicture = WebPPictureImportRGBA(
      &_webp.picture,
      _pixels.data(),
      _options.imageSpecification.size.width *
          image::pixel::specifications::RGBA.bytesPerPixel);

  _pixels.clear();

  SPECTRUM_ERROR_CSTR_IF_NOT(
      didImportPicture,
      codecs::error::CompressorFailure,
      "webp_picture_import_failed");

  const auto didEncodePicture =
      WebPEncode(&_webp.configuration, &_webp.picture);

  SPECTRUM_ERROR_CSTR_IF_NOT(
      didEncodePicture, codecs::error::CompressorFailure, "webp_encode_failed");

  // early free
  WebPPictureFree(&_webp.picture);
}

void LibWebpCompressor::_ensureHeaderWritten() {
  if (_wasHeaderWritten) {
    return;
  }

  switch (_options.encodeRequirement->mode) {
    case requirements::Encode::Mode::Lossy: {
      _webp.configuration.lossless = 0;
      _webp.configuration.quality = _quality;
      break;
    }

    case requirements::Encode::Mode::Lossless:
    case requirements::Encode::Mode::Any: {
      _webp.configuration.lossless = 1;
      _webp.configuration.quality = 20; // fast, larger filesize.
      break;
    }
  }

  _wasHeaderWritten = true;
}

} // namespace webp
} // namespace plugins
} // namespace spectrum
} // namespace facebook
