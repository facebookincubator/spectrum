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
#include <ivf/ivfheader.h>

#include <memory>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace avif {

namespace {
constexpr int DAV1D_OK = 0;

folly::Range<const std::uint8_t*> readRangeFromSource(
    io::IImageSource& source,
    const size_t len) {
  std::vector<char> bytes(len);
  SPECTRUM_ENFORCE_IF(source.read(bytes.data(), len) != len);

  const auto data = reinterpret_cast<const std::uint8_t*>(bytes.data());
  return folly::Range<const std::uint8_t*>(data, data + len);
}

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

void IvfAv1Decompressor::ensureHeaderIsRead() {
  if (_imageSpecification.hasValue()) {
    return;
  }

  auto buffer = readRangeFromSource(
      _source,
      sizeof(fb::ivf::IvfFileHeader) + sizeof(fb::ivf::IvfFrameHeader));

  // Read IvfFileHeader
  const auto fileHeader = fb::ivf::parseIvfFileHeader(buffer);
  SPECTRUM_ENFORCE_IF_NOT(fileHeader.frameCount == 1);

  // Read IvfFrameHeader
  const auto frameHeader = fb::ivf::parseIvfFrameHeader(buffer);
  SPECTRUM_ENFORCE_IF_NOT(frameHeader.payloadLength > 0);

  _imageSpecification = image::Specification{
      .size = image::Size{fileHeader.frameWidth, fileHeader.frameHeight},
      .format = formats::IvfAv1,
      .pixelSpecification = image::pixel::specifications::RGB};
}

//
// Public
//

image::Specification IvfAv1Decompressor::sourceImageSpecification() {
  ensureHeaderIsRead();
  return *_imageSpecification;
}

image::Specification IvfAv1Decompressor::outputImageSpecification() {
  return sourceImageSpecification();
}

std::unique_ptr<image::Scanline> IvfAv1Decompressor::readScanline() {
  return nullptr;
}

} // namespace avif
} // namespace plugins
} // namespace spectrum
} // namespace facebook
