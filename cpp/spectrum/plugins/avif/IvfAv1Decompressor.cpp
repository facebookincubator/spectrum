// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "IvfAv1Decompressor.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/plugins/avif/LibAvifTranscodingPlugin.h>

#include <dav1d/dav1d.h>
#include <folly/Optional.h>
#include <folly/ScopeGuard.h>
#include <ivf/ivfheader.h>
#include <libyuv/convert_from.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {

namespace {
constexpr int DAV1D_OK = 0;
} // namespace

IvfAv1Decompressor::IvfAv1Decompressor(io::IImageSource& source)
    : _source(source) {
  dav1d_default_settings(&_dav1dSettings);
  SPECTRUM_ENFORCE_IF(DAV1D_OK != dav1d_open(&_dav1dContext, &_dav1dSettings));
}

IvfAv1Decompressor::~IvfAv1Decompressor() {
  if (_dav1dContext != nullptr) {
    dav1d_close(&_dav1dContext);
  }
}

//
// Private
//

void IvfAv1Decompressor::_ensureEntireImageIsRead() {
  if (_entireImageHasBeenRead) {
    return;
  }
  _entireImageHasBeenRead = true;

  // read entire remaining image source
  std::vector<std::uint8_t> payload(_payloadLength);
  const auto bytesRead =
      _source.read(reinterpret_cast<char*>(payload.data()), _payloadLength);
  SPECTRUM_ERROR_CSTR_IF_NOT(
      bytesRead == _payloadLength,
      codecs::error::DecompressorFailure,
      "actual AV1 payload was shorter than advertised in IVF header");

  Dav1dData dav1dData{0};
  SPECTRUM_ERROR_CSTR_IF_NOT(
      DAV1D_OK ==
          dav1d_data_wrap(
              &dav1dData,
              payload.data(),
              payload.size(),
              [](const uint8_t*, void*) { /* no-op free */ },
              nullptr),
      codecs::error::DecompressorFailure,
      "failed dav1d_data_wrap");
  SCOPE_EXIT {
    dav1d_data_unref(&dav1dData);
  };

  SPECTRUM_ERROR_CSTR_IF_NOT(
      DAV1D_OK == dav1d_send_data(_dav1dContext, &dav1dData),
      codecs::error::DecompressorFailure,
      "failed dav1d_send_data");

  std::vector<std::uint8_t> rgbImage;
  std::uint32_t rgbStride, totalRgbPixels, width, height;
  {
    Dav1dPicture dav1dPicture{0};
    SPECTRUM_ERROR_CSTR_IF_NOT(
        DAV1D_OK == dav1d_get_picture(_dav1dContext, &dav1dPicture),
        codecs::error::DecompressorFailure,
        "failed dav1d_get_picture");
    SCOPE_EXIT {
      dav1d_picture_unref(&dav1dPicture);
    };

    SPECTRUM_ERROR_CSTR_IF_NOT(
        dav1dPicture.p.bpc == 8,
        codecs::error::DecompressorFailure,
        "can only read 8-bit images");

    SPECTRUM_ERROR_CSTR_IF_NOT(
        dav1dPicture.p.layout == DAV1D_PIXEL_LAYOUT_I420,
        codecs::error::DecompressorFailure,
        "can only read I420 images");

    width = dav1dPicture.p.w;
    height = dav1dPicture.p.h;
    SPECTRUM_ERROR_CSTR_IF_NOT(
        _imageSpecification->size.width == width &&
            _imageSpecification->size.height == height,
        codecs::error::DecompressorFailure,
        "payload dimensions do not match IVF information");

    rgbStride = dav1dPicture.p.w * 3;
    totalRgbPixels = rgbStride * dav1dPicture.p.h;
    rgbImage.resize(totalRgbPixels);

    libyuv::I420ToRAW(
        reinterpret_cast<uint8_t*>(dav1dPicture.data[0]), // Y
        dav1dPicture.stride[0],
        reinterpret_cast<uint8_t*>(dav1dPicture.data[1]), // U
        dav1dPicture.stride[1],
        reinterpret_cast<uint8_t*>(dav1dPicture.data[2]), // V
        dav1dPicture.stride[1], // not an index mistake: 0=luma, 1=chroma
        rgbImage.data(),
        rgbStride,
        dav1dPicture.p.w,
        dav1dPicture.p.h);

    // dav1dPicture will go out of scope here and we continue processing from
    // argbImage only; this reduces the peak memory consumption
  }

  _entireImage.reserve(height);

  for (auto row = 0; row < height; row++) {
    auto scanline = std::make_unique<image::Scanline>(
        image::pixel::specifications::RGB, width);

    const auto rgbImageOffset = row * rgbStride;
    std::copy_n(
        rgbImage.data() + rgbImageOffset,
        scanline->sizeBytes(),
        scanline->data());

    _entireImage.push_back(std::move(scanline));
  }

  // free context early
  dav1d_close(&_dav1dContext);
  _dav1dContext = nullptr;
}

void IvfAv1Decompressor::_ensureHeaderIsRead() {
  if (_imageSpecification.hasValue()) {
    return;
  }
  auto len = sizeof(fb::ivf::IvfFileHeader) + sizeof(fb::ivf::IvfFrameHeader);
  std::vector<char> bytes(len);
  SPECTRUM_ENFORCE_IF(_source.read(bytes.data(), len) != len);
  const auto data = reinterpret_cast<const std::uint8_t*>(bytes.data());
  auto buffer = folly::Range<const std::uint8_t*>(data, data + len);

  // Read IvfFileHeader
  const auto fileHeader = fb::ivf::parseIvfFileHeader(buffer);
  SPECTRUM_ENFORCE_IF_NOT(fileHeader.frameCount == 1);

  // Read IvfFrameHeader
  const auto frameHeader = fb::ivf::parseIvfFrameHeader(buffer);
  SPECTRUM_ENFORCE_IF_NOT(frameHeader.payloadLength > 0);
  _payloadLength = frameHeader.payloadLength;

  _imageSpecification = image::Specification{
      .size = image::Size{fileHeader.frameWidth, fileHeader.frameHeight},
      .format = formats::IvfAv1,
      .pixelSpecification = image::pixel::specifications::RGB};
}

//
// Public
//

image::Specification IvfAv1Decompressor::sourceImageSpecification() {
  _ensureHeaderIsRead();
  return *_imageSpecification;
}

image::Specification IvfAv1Decompressor::outputImageSpecification() {
  return sourceImageSpecification();
}

std::unique_ptr<image::Scanline> IvfAv1Decompressor::readScanline() {
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
