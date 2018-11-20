// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "FileImageSink.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSink.h>

#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>

namespace facebook {
namespace spectrum {
namespace io {

FileImageSink::FileImageSink(const std::string& path)
    : IEncodedImageSink(), ofs(path, std::fstream::binary) {
  SPECTRUM_ERROR_IF_NOT(ofs, error::ImageSinkFailure);
}

void FileImageSink::_write(const char* const source, const std::size_t length) {
  ofs.write(source, length);
}

void FileImageSink::setConfiguration(
    const image::Size& imageSize,
    const image::pixel::Specification& pixelSpecification) {
  // file don't propagate configurations
}

} // namespace io
} // namespace spectrum
} // namespace facebook
