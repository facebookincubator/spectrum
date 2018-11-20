// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/core/Constants.h>
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

/**
 * The LibJpegSinkManager allows using an ImageSource as the jpeg_source_mgr
 * in libjpeg. It wraps around an ImageSource and then needs to be registered
 * in the jpeg_decompress_struct->src field.
 */
class LibJpegSourceManager {
 public:
  explicit LibJpegSourceManager(io::IImageSource& imageSink);
  jpeg_source_mgr* getLibJpegSourceManagerPointer();

 private:
  /**
   * State for libjpeg
   */
  struct jpeg_source_mgr libJpegSourceManager = {};

  /**
   * The image source to read from
   */
  io::IImageSource& source;

  /**
   * Buffer accessed by libjpeg.
   */
  std::array<JOCTET, core::DefaultBufferSize> buffer = {};

  /**
   * Defines whether an operation has already been executed.
   */
  bool startOfFile = true;

  //
  // Hooks
  //

  static int imageSourceFillInputBuffer(j_decompress_ptr dinfo);
  static void imageSourceInitSource(j_decompress_ptr dinfo);
  static void imageSourceSkipInputData(
      j_decompress_ptr dinfo,
      long bytesToSkip);
  static void imageSourceTermSource(j_decompress_ptr dinfo);
};

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
