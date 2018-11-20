// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibPngDecompressor.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSource.h>
#include <spectrum/plugins/png/LibPngConstants.h>

#include "png.h"
#include "pngstruct.h"
// must come after pngstruct
#include "pnginfo.h"

#include <folly/Optional.h>

#include <csetjmp>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace png {

namespace {
void libPngReadCallback(
    png_structp libPngReadStruct,
    png_bytep data,
    png_size_t len) {
  auto& source =
      *static_cast<io::IImageSource*>(png_get_io_ptr(libPngReadStruct));

  static_assert(
      sizeof(png_byte) == sizeof(char), "png_byte and char differ in size");

  const auto bytesRead = source.read(reinterpret_cast<char*>(data), len);
  SPECTRUM_ERROR_IF_NOT(bytesRead == len, codecs::error::DecompressorFailure);
}

image::pixel::Specification _pixelSpecificationFromColorType(
    const png_byte colorType,
    const png_byte bitDepth,
    const int channels) {
  if (bitDepth == 8 && colorType == PNG_COLOR_TYPE_GRAY && channels == 1) {
    return image::pixel::specifications::Gray;
  } else if (
      bitDepth == 8 && colorType == PNG_COLOR_TYPE_RGB && channels == 3) {
    return image::pixel::specifications::RGB;
  } else if (
      bitDepth == 8 && colorType == PNG_COLOR_TYPE_RGBA && channels == 4) {
    return image::pixel::specifications::ARGB;
  } else {
    SPECTRUM_ERROR_FORMAT(
        codecs::error::DecompressorUnexpectedPixelSpecification,
        "colorType: %u, bitDepth: %u, channels: %u",
        colorType,
        bitDepth,
        channels);
  }
}
} // namespace

// allows to define a static error handler function that can be "friended"
// with LibPngDecompressor to access the private setErrorMessage method
struct LibPngDecompressor::LibPngDecompressorErrorHandler {
  static void libPngErrorHandler(png_structp png_ptr, png_const_charp msg) {
    png_voidp error_ptr = png_get_error_ptr(png_ptr);
    LibPngDecompressor* libPngDecompressor =
        static_cast<LibPngDecompressor*>(error_ptr);
    libPngDecompressor->setErrorMessage(std::string(msg));

    longjmp(png_jmpbuf(png_ptr), 1);
  }
};

LibPngDecompressor::LibPngDecompressor(
    io::IImageSource& source_,
    const folly::Optional<image::Ratio>& samplingRatio)
    : source(source_) {
  IDecompressor::_ensureNoSamplingRatio(samplingRatio);

  libPngReadStruct = png_create_read_struct(
      PNG_LIBPNG_VER_STRING,
      /* error_ptr */ this,
      /* error_fn  */ &LibPngDecompressorErrorHandler::libPngErrorHandler,
      /* warn_fn   */ nullptr);

  SPECTRUM_ERROR_CSTR_IF(
      libPngReadStruct == nullptr,
      codecs::error::DecompressorFailure,
      "png_create_read_struct_failed");

  libPngInfoStruct = png_create_info_struct(libPngReadStruct);

  if (!libPngInfoStruct) {
    png_destroy_read_struct(&libPngReadStruct, nullptr, nullptr);
    SPECTRUM_ERROR_CSTR(
        codecs::error::DecompressorFailure, "png_create_info_struct_failed");
  }

  libPngEndInfoStruct = png_create_info_struct(libPngReadStruct);
  if (!libPngEndInfoStruct) {
    png_destroy_read_struct(&libPngReadStruct, &libPngInfoStruct, nullptr);
    SPECTRUM_ERROR_CSTR(
        codecs::error::DecompressorFailure, "png_create_info_struct_failed");
  }

  png_set_read_fn(libPngReadStruct, &source, &libPngReadCallback);

  png_set_user_limits(
      libPngReadStruct, maximumSizeDimension, maximumSizeDimension);
}

LibPngDecompressor::~LibPngDecompressor() {
  png_destroy_read_struct(
      &libPngReadStruct, &libPngInfoStruct, &libPngEndInfoStruct);
}

//
// Private
//

void LibPngDecompressor::ensureHeaderIsRead() {
  // only read and re-configure header once
  if (isHeaderRead) {
    return;
  }

  if (setjmp(png_jmpbuf(libPngReadStruct))) {
    throwError(__PRETTY_FUNCTION__, __LINE__, "png_read_info");
  }
  png_read_info(libPngReadStruct, libPngInfoStruct);

  // expand to bitDepth 8
  // expand palette to RGB or ARGB
  png_set_expand(libPngReadStruct);

  // reduce bitDepth to 8 bit if necessary
  if (libPngInfoStruct->bit_depth == 16) {
    png_set_strip_16(libPngReadStruct);
  }

  // this will update the libPngInfoStruct with the information as they would be
  // after the conversion. I.e.: an originally 4bite-palette PNG is now showing
  // as a 8bit RGB PNG.
  if (setjmp(png_jmpbuf(libPngReadStruct))) {
    throwError(__PRETTY_FUNCTION__, __LINE__, "png_read_update_info");
  }
  png_read_update_info(libPngReadStruct, libPngInfoStruct);

  // libPng by default outputs RGBA while we work with ARGB
  // png_set_swap_alpha needs to be called after png_read_update_info() in order
  // to also include palette to RGBA transformations that change the colorType
  if (libPngInfoStruct->color_type & PNG_COLOR_MASK_ALPHA) {
    png_set_swap_alpha(libPngReadStruct);
  }

  isHeaderRead = true;
}

void LibPngDecompressor::ensureReadyToReadScanline() {
  ensureHeaderIsRead();

  SPECTRUM_ENFORCE_IF_NOT(outputScanline < libPngInfoStruct->height);

  const auto interlaceType = libPngInfoStruct->interlace_type;
  isInterlaced = interlaceType != PNG_INTERLACE_NONE;
}

std::unique_ptr<image::Scanline> LibPngDecompressor::readOneLine() {
  const auto imageSpecification = sourceImageSpecification();
  auto scanline = std::make_unique<image::Scanline>(
      imageSpecification.pixelSpecification, imageSpecification.size.width);

  if (setjmp(png_jmpbuf(libPngReadStruct))) {
    throwError(__PRETTY_FUNCTION__, __LINE__, "png_read_row");
  }
  png_read_row(
      libPngReadStruct, reinterpret_cast<png_bytep>(scanline->data()), nullptr);

  ++outputScanline;
  return scanline;
}

void LibPngDecompressor::ensureEntireImageIsRead(
    std::vector<std::unique_ptr<image::Scanline>>* imageVector) {
  const auto imageSpecification = sourceImageSpecification();
  const auto imageSize = imageSpecification.size;

  if (imageVector->size() == imageSize.height) {
    return;
  }

  imageVector->reserve(imageSize.height);

  auto rowPointers = std::make_unique<png_bytep[]>(imageSize.height);

  for (std::size_t row = 0; row < imageSize.height; row++) {
    imageVector->push_back(std::make_unique<image::Scanline>(
        imageSpecification.pixelSpecification, imageSize.width));
    rowPointers[row] = reinterpret_cast<png_bytep>(imageVector->back()->data());
    SPECTRUM_ENFORCE_IF_NOT(rowPointers[row] != nullptr);
  }

  if (setjmp(png_jmpbuf(libPngReadStruct))) {
    throwError(__PRETTY_FUNCTION__, __LINE__, "png_read_image");
  }
  png_read_image(libPngReadStruct, rowPointers.get());
}

void LibPngDecompressor::setErrorMessage(const std::string& errorMessage) {
  this->errorMessage = errorMessage;
}

void LibPngDecompressor::throwError(
    const char* const function,
    const unsigned int line,
    const char* const culprit) {
  auto message = std::string(culprit) + std::string("_failed");

  if (errorMessage.hasValue()) {
    message += ": " + *errorMessage;
  }

  throw SpectrumException(
      codecs::error::DecompressorFailure, message, function, line);
}

//
// Decompressor
//

image::Specification LibPngDecompressor::sourceImageSpecification() {
  if (_sourceImageSpecification.hasValue()) {
    return *_sourceImageSpecification;
  }

  ensureHeaderIsRead();

  const auto pixelSpecification = _pixelSpecificationFromColorType(
      libPngInfoStruct->color_type,
      libPngInfoStruct->bit_depth,
      png_get_channels(libPngReadStruct, libPngInfoStruct));

  _sourceImageSpecification = image::Specification{
      .size = image::Size{SPECTRUM_CONVERT_OR_THROW(
                              libPngInfoStruct->width, std::uint32_t),
                          SPECTRUM_CONVERT_OR_THROW(
                              libPngInfoStruct->height, std::uint32_t)},
      .format = image::formats::Png,
      .pixelSpecification = pixelSpecification,
      .chromaSamplingMode = image::ChromaSamplingMode::S444,
  };

  return *_sourceImageSpecification;
}

image::Specification LibPngDecompressor::outputImageSpecification() {
  return sourceImageSpecification();
}

std::unique_ptr<image::Scanline> LibPngDecompressor::readScanline() {
  ensureReadyToReadScanline();

  if (isInterlaced) {
    // if interlaced, we must read the entire image to get the final rows
    ensureEntireImageIsRead(&entireImage);
    return std::move(entireImage[outputScanline++]);
  } else {
    return readOneLine();
  }
}

} // namespace png
} // namespace plugins
} // namespace spectrum
} // namespace facebook
