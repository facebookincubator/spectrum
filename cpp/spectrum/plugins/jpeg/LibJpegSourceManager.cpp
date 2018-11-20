// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegSourceManager.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSource.h>

#include <array>
#include <memory>

#include <mozjpeg/jerror.h>
#include <mozjpeg/jinclude.h>
#include <mozjpeg/jpeglib.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

LibJpegSourceManager::LibJpegSourceManager(io::IImageSource& source)
    : source(source) {
  // register static hooks
  libJpegSourceManager.init_source =
      LibJpegSourceManager::imageSourceInitSource;
  libJpegSourceManager.fill_input_buffer =
      LibJpegSourceManager::imageSourceFillInputBuffer;
  libJpegSourceManager.skip_input_data =
      LibJpegSourceManager::imageSourceSkipInputData;
  libJpegSourceManager.resync_to_restart =
      jpeg_resync_to_restart; // default method
  libJpegSourceManager.term_source =
      LibJpegSourceManager::imageSourceTermSource;

  // set initial buffer state that forces a fill_input_buffer call when trying
  // to read for the first time
  libJpegSourceManager.bytes_in_buffer = 0;
  libJpegSourceManager.next_input_byte = nullptr;
}

jpeg_source_mgr* LibJpegSourceManager::getLibJpegSourceManagerPointer() {
  return &libJpegSourceManager;
}

//
// Hooks
//

void LibJpegSourceManager::imageSourceInitSource(
    j_decompress_ptr /* unused */) {
  // ignore (life cycle outside libjpeg's scope)
}

int LibJpegSourceManager::imageSourceFillInputBuffer(j_decompress_ptr dinfo) {
  auto src = reinterpret_cast<LibJpegSourceManager*>(dinfo->src);

  static_assert(
      sizeof(JOCTET) == sizeof(char), "JOCTET and char have different size");
  std::size_t bytesRead = src->source.read(
      reinterpret_cast<char*>(src->buffer.data()), src->buffer.size());

  if (bytesRead == 0) {
    if (src->startOfFile) {
      ERREXIT(dinfo, JERR_INPUT_EMPTY);
    }

    // It is recommended by libjpeg to add the EOI tag when encountering an
    // "unexpected" end of input
    src->buffer[0] = (JOCTET)0xFF;
    src->buffer[1] = (JOCTET)JPEG_EOI;
    bytesRead = 2;
  }

  src->startOfFile = false;

  src->libJpegSourceManager.next_input_byte = src->buffer.data();
  src->libJpegSourceManager.bytes_in_buffer = bytesRead;
  return true;
}

void LibJpegSourceManager::imageSourceSkipInputData(
    j_decompress_ptr dinfo,
    long pBytesToSkip) {
  auto src = reinterpret_cast<LibJpegSourceManager*>(dinfo->src);

  SPECTRUM_ENFORCE_IF_NOT(pBytesToSkip > 0);
  std::size_t bytesToSkip = static_cast<std::size_t>(pBytesToSkip);

  if (bytesToSkip > 0) {
    if (src->libJpegSourceManager.bytes_in_buffer > bytesToSkip) {
      // skip by advancing the buffer
      src->libJpegSourceManager.next_input_byte += bytesToSkip;
      src->libJpegSourceManager.bytes_in_buffer -= bytesToSkip;
    } else {
      // skip by advancing the buffer
      bytesToSkip = bytesToSkip - src->libJpegSourceManager.bytes_in_buffer;

      // and skipping the source
      while (bytesToSkip > 0) {
        const std::size_t bytesToRead =
            std::min(bytesToSkip, src->buffer.size());

        static_assert(
            sizeof(JOCTET) == sizeof(char),
            "JOCTET and char have different size");
        std::size_t bytesSkipped = src->source.read(
            reinterpret_cast<char*>(src->buffer.data()), bytesToRead);
        bytesToSkip -= bytesSkipped;

        SPECTRUM_ENFORCE_IF_NOT(bytesSkipped > 0);
      }

      src->libJpegSourceManager.next_input_byte = nullptr;
      src->libJpegSourceManager.bytes_in_buffer = 0;
    }
  }
}

void LibJpegSourceManager::imageSourceTermSource(
    j_decompress_ptr /* unused */) {
  // ignore (life cycle outside libjpeg's scope)
}

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
