// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

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

/**
 * The LibJpegSinkManager allows using an ImageSink as the jpeg_destination_mgr
 * in libjpeg. It wraps around an ImageSink and then needs to be registered
 * in the jpeg_compress_struct->dest field.
 */
class LibJpegSinkManager {
 public:
  explicit LibJpegSinkManager(io::IImageSink& imageSink);
  jpeg_destination_mgr* getLibJpegDestinationManagerPointer();

 private:
  /**
   * State of libjpeg.
   */
  struct jpeg_destination_mgr libJpegDestinationManager = {};

  /**
   * Pointer to the ImageSink
   */
  io::IImageSink& sink;

  /**
   * Buffer accessed by libjpeg.
   */
  std::array<JOCTET, core::DefaultBufferSize> buffer = {};

  //
  // Hooks
  //

  static void imageSinkInitDestination(j_compress_ptr dinfo);
  static int imageSinkEmptyBuffer(j_compress_ptr dinfo);
  static void imageSinkTermDestination(j_compress_ptr dinfo);
  static void writeBufferToSink(
      LibJpegSinkManager* sinkManager,
      io::IImageSink& imageSink,
      const std::size_t bytesToWrite);
};

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
