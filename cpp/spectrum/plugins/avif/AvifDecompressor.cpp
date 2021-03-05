// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "AvifDecompressor.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/plugins/avif/LibAvifTranscodingPlugin.h>

#include <avif/avif.h>
#include <folly/ScopeGuard.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {

namespace {

std::vector<std::uint8_t> readEntireImageSource(io::IImageSource& source) {
  std::vector<std::uint8_t> out{};
  out.reserve(source.available());

  std::vector<std::uint8_t> buffer(core::DefaultBufferSize);
  std::size_t bytesRead;
  while ((bytesRead = source.read(
              reinterpret_cast<char* const>(buffer.data()), buffer.size())) >
         0) {
    out.insert(out.end(), buffer.begin(), buffer.begin() + bytesRead);
  }

  return out;
}

} // namespace

AvifDecompressor::AvifDecompressor(io::IImageSource& source)
    : _source(source) {}

AvifDecompressor::~AvifDecompressor() {
  if (_decoder) {
    avifDecoderDestroy(_decoder);
  }
}

//
// Private
//

void AvifDecompressor::_parseContainer() {
  if (_decoder) {
    return;
  }

  _sourceData = readEntireImageSource(_source);

  _decoder = avifDecoderCreate();

  SPECTRUM_ERROR_CSTR_IF_NOT(
      AVIF_RESULT_OK ==
          avifDecoderSetIOMemory(
              _decoder, _sourceData.data(), _sourceData.size()),
      codecs::error::DecompressorFailure,
      "failed avifDecoderSetIOMemory");

  SPECTRUM_ERROR_CSTR_IF_NOT(
      AVIF_RESULT_OK == avifDecoderParse(_decoder) &&
          _decoder->image != nullptr,
      codecs::error::DecompressorFailure,
      "failed avifDecoderParse");
}

void AvifDecompressor::_computeSpecifications() {
  if (_imageSpecification) {
    return;
  }

  _parseContainer();
  _imageSpecification = image::Specification{
      .size = image::Size{_decoder->image->width, _decoder->image->height},
      .format = image::formats::Avif,
      .pixelSpecification = image::pixel::specifications::RGB};
}

void AvifDecompressor::_ensureEntireImageIsRead() {
  if (_entireImageHasBeenRead) {
    return;
  }
  _entireImageHasBeenRead = true;

  _parseContainer();

  SPECTRUM_ERROR_CSTR_IF_NOT(
      AVIF_RESULT_OK == avifDecoderNextImage(_decoder) && _decoder->image,
      codecs::error::DecompressorFailure,
      "failed avifDecoderNextImage");
  _sourceData.clear();

  const auto image = _decoder->image;

  avifRGBImage rgb;
  avifRGBImageSetDefaults(&rgb, image);
  rgb.depth = 8;
  rgb.format = AVIF_RGB_FORMAT_RGB;

  avifRGBImageAllocatePixels(&rgb);
  SCOPE_EXIT {
    avifRGBImageFreePixels(&rgb);
  };

  SPECTRUM_ERROR_CSTR_IF_NOT(
      AVIF_RESULT_OK == avifImageYUVToRGB(image, &rgb),
      codecs::error::DecompressorFailure,
      "failed avifImageYUVToRGB");

  _entireImage.reserve(image->height);
  for (auto row = 0; row < image->height; ++row) {
    auto scanline = std::make_unique<image::Scanline>(
        image::pixel::specifications::RGB, image->width);

    SPECTRUM_ERROR_FORMAT_IF_NOT(
        scanline->sizeBytes() == rgb.rowBytes,
        codecs::error::DecompressorFailure,
        "scanline size (%d) does not match decoded row bytes (%d)",
        scanline->sizeBytes(),
        rgb.rowBytes);

    std::memcpy(
        scanline->data(), rgb.pixels + row * rgb.rowBytes, rgb.rowBytes);
    _entireImage.push_back(std::move(scanline));
  }

  _imageSpecification = image::Specification{
      .size = image::Size{image->width, image->height},
      .format = image::formats::Avif,
      .pixelSpecification = image::pixel::specifications::RGB};

  // We are done with the decoder, free it now to save memory
  avifDecoderDestroy(_decoder);
  _decoder = nullptr;
}

//
// Public
//

image::Specification AvifDecompressor::sourceImageSpecification() {
  _computeSpecifications();
  return *_imageSpecification;
}

image::Specification AvifDecompressor::outputImageSpecification() {
  return sourceImageSpecification();
}

std::unique_ptr<image::Scanline> AvifDecompressor::readScanline() {
  _ensureEntireImageIsRead();
  std::unique_ptr<image::Scanline> result;

  if (_currentOutputScanline < _imageSpecification->size.height) {
    result = std::move(_entireImage[_currentOutputScanline]);
    _currentOutputScanline++;
  }

  // free memory early
  if (_currentOutputScanline == _imageSpecification->size.height) {
    _entireImage.clear();
  }

  return result;
}

} // namespace avif
} // namespace plugins
} // namespace spectrum
} // namespace facebook
