// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "FileImageSource.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSource.h>

#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>

namespace facebook {
namespace spectrum {
namespace io {

FileImageSource::FileImageSource(const std::string& path)
    : IEncodedImageSource(), ifs(path, std::fstream::binary) {
  SPECTRUM_ERROR_IF_NOT(ifs, error::ImageSourceFailure);
}

std::size_t FileImageSource::read(
    char* const destination,
    const std::size_t length) {
  this->ifs.read(destination, length);

  const std::size_t bytesRead = this->ifs.gcount();
  this->totalBytesRead += bytesRead;

  return bytesRead;
}

std::size_t FileImageSource::available() {
  const auto originalPosition = ifs.tellg();
  SPECTRUM_ENFORCE_IF_NOT(originalPosition >= 0);

  // seek to end position of input file stream
  ifs.seekg(0, ifs.end);
  const auto endPosition = ifs.tellg();

  ifs.seekg(originalPosition, ifs.beg);
  return endPosition - originalPosition;
}

std::size_t FileImageSource::getTotalBytesRead() const {
  return totalBytesRead;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
