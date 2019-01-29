// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibPngCompressor.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/image/Scanline.h>
#include <spectrum/io/IImageSink.h>
#include <spectrum/plugins/png/LibPngConstants.h>

#include <csetjmp>
#include <cstdint>
#include <exception>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "png.h"

namespace facebook {
namespace spectrum {
namespace plugins {
namespace png {

namespace /* anonymous */ {
void libPngWriteCallback(
    png_structp libPngWriteStruct,
    png_bytep data,
    png_size_t len) {
  io::IImageSink* sink =
      static_cast<io::IImageSink*>(png_get_io_ptr(libPngWriteStruct));
  SPECTRUM_ENFORCE_IF_NOT(sink != nullptr);

  static_assert(
      sizeof(png_byte) == sizeof(char), "png_byte and char differ in size");
  sink->write(reinterpret_cast<char*>(data), len);
}

int colorTypeFromPixelSpecification(
    const image::pixel::Specification pixelSpecification) {
  if (pixelSpecification == image::pixel::specifications::Gray) {
    return PNG_COLOR_TYPE_GRAY;

  } else if (pixelSpecification == image::pixel::specifications::RGB) {
    return PNG_COLOR_TYPE_RGB;

  } else if (pixelSpecification == image::pixel::specifications::ARGB) {
#ifdef PNG_FORMAT_AFIRST_SUPPORTED
    return PNG_COLOR_TYPE_RGBA | PNG_FORMAT_FLAG_AFIRST;
#else
    return PNG_COLOR_TYPE_RGBA;
#endif

  } else if (pixelSpecification == image::pixel::specifications::RGBA) {
    return PNG_COLOR_TYPE_RGBA;

  } else {
    SPECTRUM_ERROR_STRING(
        codecs::error::CompressorCannotWritePixelSpecification,
        pixelSpecification.string());
  }
}
} // anonymous namespace

// allows to define a static error handler function that can be "friended"
// with LibPngDecompressor to access the private setErrorMessage method
struct LibPngCompressor::LibPngCompressorErrorHandler {
  static void libPngErrorHandler(png_structp png_ptr, png_const_charp msg) {
    png_voidp error_ptr = png_get_error_ptr(png_ptr);
    LibPngCompressor* libPngCompressor =
        static_cast<LibPngCompressor*>(error_ptr);
    libPngCompressor->setErrorMessage(std::string(msg));

    longjmp(png_jmpbuf(png_ptr), 1);
  }
};

LibPngCompressor::LibPngCompressor(const codecs::CompressorOptions& options)
    : _options(options) {
  ICompressor::enforceLossless(options.encodeRequirement);
  ICompressor::enforceCannotEncodeMetadata(options.imageSpecification.metadata);
  ICompressor::enforceSizeBelowMaximumSideDimension(
      options.imageSpecification.size, maximumSizeDimension);

  // propagate image configuration
  options.sink.setConfiguration(
      options.imageSpecification.size,
      options.imageSpecification.pixelSpecification);

  libPngWriteStruct = png_create_write_struct(
      PNG_LIBPNG_VER_STRING,
      /* error_ptr */ this,
      /* error_fn  */ &LibPngCompressorErrorHandler::libPngErrorHandler,
      /* warn_fn   */ nullptr);

  SPECTRUM_ERROR_CSTR_IF(
      libPngWriteStruct == nullptr,
      codecs::error::CompressorFailure,
      "png_create_write_struct_failed");

  libPngInfoStruct = png_create_info_struct(libPngWriteStruct);
  if (!libPngInfoStruct) {
    png_destroy_write_struct(&libPngWriteStruct, (png_infopp) nullptr);
    SPECTRUM_ERROR_CSTR(
        codecs::error::CompressorFailure, "png_create_info_struct_failed");
  }

  png_set_compression_level(
      libPngWriteStruct, options.configuration.png.compressionLevel());

  png_set_write_fn(
      libPngWriteStruct,
      &options.sink,
      &libPngWriteCallback,
      (png_flush_ptr) nullptr);
};

LibPngCompressor::~LibPngCompressor() {
  png_destroy_write_struct(&libPngWriteStruct, &libPngInfoStruct);
}

//
// Private
//

void LibPngCompressor::ensureHeaderIsWritten(std::uint16_t colorType) {
  SPECTRUM_ENFORCE_IF_NOT(!writtenLastScanline);

  if (isHeaderWritten) {
    return;
  }

  if (setjmp(png_jmpbuf(libPngWriteStruct))) {
    throwError(__PRETTY_FUNCTION__, __LINE__, "png_set_IHDR");
  }

  png_set_IHDR(
      libPngWriteStruct,
      libPngInfoStruct,
      _options.imageSpecification.size.width,
      _options.imageSpecification.size.height,
      8,
      colorType,
      _options.configuration.png.useInterlacing() ? PNG_INTERLACE_ADAM7
                                                  : PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT,
      PNG_FILTER_TYPE_DEFAULT);

  isHeaderWritten = true;

  if (setjmp(png_jmpbuf(libPngWriteStruct))) {
    throwError(__PRETTY_FUNCTION__, __LINE__, "png_write_info");
  }
  png_write_info(libPngWriteStruct, libPngInfoStruct);
}

void LibPngCompressor::finishIfLastScanlineWritten() {
  if (!writtenLastScanline &&
      inputScanline == _options.imageSpecification.size.height) {
    writtenLastScanline = true;

    if (setjmp(png_jmpbuf(libPngWriteStruct))) {
      throwError(__PRETTY_FUNCTION__, __LINE__, "png_write_end");
    }
    png_write_end(libPngWriteStruct, (png_infop) nullptr);
  }
}

void LibPngCompressor::internalWriteScanlineBaseline(
    std::unique_ptr<image::Scanline> scanline) {
  if (setjmp(png_jmpbuf(libPngWriteStruct))) {
    throwError(__PRETTY_FUNCTION__, __LINE__, "png_write_row");
  }
  png_write_row(
      libPngWriteStruct, reinterpret_cast<png_bytep>(scanline->data()));

  inputScanline++;
  finishIfLastScanlineWritten();
}

void LibPngCompressor::internalWriteScanlineInterlaced(
    std::unique_ptr<image::Scanline> scanline) {
  // buffer incoming scanlines
  if (interlaceScanlineBuffer.size() == 0) {
    interlaceScanlineBuffer.reserve(_options.imageSpecification.size.height);
  }
  interlaceScanlineBuffer.push_back(std::move(scanline));
  inputScanline++;

  // write all of them once the complete image has been buffered
  if (interlaceScanlineBuffer.size() ==
      _options.imageSpecification.size.height) {
    if (setjmp(png_jmpbuf(libPngWriteStruct))) {
      throwError(__PRETTY_FUNCTION__, __LINE__, "png_write_row");
    }

    const auto numberPasses = png_set_interlace_handling(libPngWriteStruct);
    for (auto currentPass = 0; currentPass < numberPasses; currentPass++) {
      for (auto& currentScanline : interlaceScanlineBuffer) {
        png_write_row(
            libPngWriteStruct,
            reinterpret_cast<png_bytep>(currentScanline->data()));
      }
    }

    interlaceScanlineBuffer.clear();
    finishIfLastScanlineWritten();
  }
}

void LibPngCompressor::setErrorMessage(const std::string& errorMessage) {
  this->errorMessage = errorMessage;
}

void LibPngCompressor::throwError(
    const char* const function,
    const unsigned int line,
    const char* const culprit) {
  auto message = std::string(culprit) + std::string("_failed");

  if (errorMessage.hasValue()) {
    message += ": " + *errorMessage;
  }

  throw SpectrumException(
      codecs::error::CompressorFailure, message, function, line);
}

//
// Compressor
//

void LibPngCompressor::writeScanline(
    std::unique_ptr<image::Scanline> scanline) {
  const auto pixelSpecification = scanline->specification();
  if (pixelSpecification == image::pixel::specifications::Gray ||
      pixelSpecification == image::pixel::specifications::RGB ||
      pixelSpecification == image::pixel::specifications::ARGB) {
    ensureHeaderIsWritten(colorTypeFromPixelSpecification(pixelSpecification));

    SPECTRUM_ENFORCE_IF_NOT(
        pixelSpecification == _options.imageSpecification.pixelSpecification);
    SPECTRUM_ENFORCE_IF_NOT(
        scanline->width() == _options.imageSpecification.size.width);
    SPECTRUM_ENFORCE_IF(writtenLastScanline);

    if (_options.configuration.png.useInterlacing()) {
      return internalWriteScanlineInterlaced(std::move(scanline));
    } else {
      return internalWriteScanlineBaseline(std::move(scanline));
    }
  } else {
    SPECTRUM_ERROR_STRING(
        codecs::error::CompressorCannotWritePixelSpecification,
        pixelSpecification.string());
  }
}

} // namespace png
} // namespace plugins
} // namespace spectrum
} // namespace facebook
