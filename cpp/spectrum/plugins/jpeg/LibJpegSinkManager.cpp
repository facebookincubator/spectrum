// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "LibJpegSinkManager.h"

#include <spectrum/core/Constants.h>
#include <spectrum/io/IImageSink.h>

#include <array>
#include <memory>

#include <mozjpeg/jerror.h>
#include <mozjpeg/jinclude.h>
#include <mozjpeg/jpeglib.h>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

LibJpegSinkManager::LibJpegSinkManager(io::IImageSink& sink) : sink(sink) {
  // register static hooks
  libJpegDestinationManager.init_destination =
      LibJpegSinkManager::imageSinkInitDestination;
  libJpegDestinationManager.empty_output_buffer =
      LibJpegSinkManager::imageSinkEmptyBuffer;
  libJpegDestinationManager.term_destination =
      LibJpegSinkManager::imageSinkTermDestination;

  // set initial buffer state
  libJpegDestinationManager.next_output_byte = buffer.data();
  libJpegDestinationManager.free_in_buffer = buffer.size();
}

jpeg_destination_mgr*
LibJpegSinkManager::getLibJpegDestinationManagerPointer() {
  return &libJpegDestinationManager;
}

void LibJpegSinkManager::imageSinkInitDestination(j_compress_ptr /* unused */) {
  // ignore (life cycle outside libjpeg's scope)
}

int LibJpegSinkManager::imageSinkEmptyBuffer(j_compress_ptr cinfo) {
  auto dest = reinterpret_cast<LibJpegSinkManager*>(cinfo->dest);

  // this hook should ignore the `free_in_buffer` field and always write
  // the entire buffer
  const std::size_t bytesToWrite = dest->buffer.size();
  writeBufferToSink(dest, dest->sink, bytesToWrite);

  return true;
}

void LibJpegSinkManager::imageSinkTermDestination(j_compress_ptr cinfo) {
  auto dest = reinterpret_cast<LibJpegSinkManager*>(cinfo->dest);

  const std::size_t freeBytes = dest->libJpegDestinationManager.free_in_buffer;
  SPECTRUM_ENFORCE_IF_NOT(dest->buffer.size() >= freeBytes);

  const std::size_t bytesToWrite = dest->buffer.size() - freeBytes;
  SPECTRUM_ENFORCE_IF_NOT(bytesToWrite <= dest->buffer.size());

  // flush remaining buffer
  writeBufferToSink(dest, dest->sink, bytesToWrite);
}

void LibJpegSinkManager::writeBufferToSink(
    LibJpegSinkManager* sinkManager,
    io::IImageSink& imageSink,
    const std::size_t bytesToWrite) {
  static_assert(
      sizeof(JOCTET) == sizeof(char), "JOCTET and char have different size");

  SPECTRUM_ENFORCE_IF_NOT(bytesToWrite <= sinkManager->buffer.size());
  imageSink.write(
      reinterpret_cast<char*>(sinkManager->buffer.data()), bytesToWrite);

  sinkManager->libJpegDestinationManager.next_output_byte =
      sinkManager->buffer.data();
  sinkManager->libJpegDestinationManager.free_in_buffer =
      sinkManager->buffer.size();
}

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
