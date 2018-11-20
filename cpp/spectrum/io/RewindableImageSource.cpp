// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "RewindableImageSource.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <vector>

namespace facebook {
namespace spectrum {
namespace io {

RewindableImageSource::RewindableImageSource(IImageSource& imageSource)
    : IEncodedImageSource(), imageSource(imageSource) {}

std::size_t RewindableImageSource::read(
    char* const destination,
    const std::size_t length) {
  std::size_t bytesLeftToCopy = length;
  std::size_t bytesCopied = 0;

  if (offset > 0) {
    // buffer >> destination
    const std::size_t elementsLeftInBuffer = offset;
    const std::size_t elementsToCopyFromBuffer =
        std::min(length, elementsLeftInBuffer);

    SPECTRUM_ENFORCE_IF_NOT(buffer.size() >= offset);
    std::copy_n(buffer.end() - offset, elementsToCopyFromBuffer, destination);

    SPECTRUM_ENFORCE_IF_NOT(offset >= elementsToCopyFromBuffer);
    SPECTRUM_ENFORCE_IF_NOT(bytesLeftToCopy >= elementsToCopyFromBuffer);
    offset -= elementsToCopyFromBuffer;
    bytesCopied += elementsToCopyFromBuffer;
    bytesLeftToCopy -= elementsToCopyFromBuffer;
  }

  if (bytesLeftToCopy == 0) {
    SPECTRUM_ENFORCE_IF_NOT(bytesCopied == length);
    return bytesCopied;
  }

  SPECTRUM_ENFORCE_IF_NOT(offset == 0);
  SPECTRUM_ENFORCE_IF_NOT(bytesLeftToCopy + bytesCopied == length);

  if (isMarkActive) {
    // imageSource >> tmp
    auto tempDst = std::unique_ptr<char[]>{new char[bytesLeftToCopy]};
    const char* tempDstPtr = tempDst.get();

    const std::size_t innerRead =
        imageSource.read(tempDst.get(), bytesLeftToCopy);
    SPECTRUM_ENFORCE_IF_NOT(innerRead <= bytesLeftToCopy);

    buffer.reserve(buffer.size() + innerRead);
    for (std::size_t i = 0; i < innerRead; i++) {
      const char c = tempDstPtr[i];

      // tmp >> buffer
      buffer.push_back(c);

      // tmp >> destination
      destination[bytesCopied + i] = c;
    }
    SPECTRUM_ENFORCE_IF_NOT(buffer.size() >= innerRead);

    bytesCopied += innerRead;
  } else {
    // // imageSource >> destination
    bytesCopied += imageSource.read(destination + bytesCopied, bytesLeftToCopy);

    if (!buffer.empty()) {
      // we are past the buffer and cannot jump back
      buffer.clear();
    }
  }
  SPECTRUM_ENFORCE_IF_NOT(bytesCopied <= length);
  return bytesCopied;
}

void RewindableImageSource::mark() {
  if (isMarkActive) {
    // erase un-reachable buffer portion (valid since we cannot move the logical
    // read-head to before the new mark)
    buffer.erase(buffer.begin(), buffer.end() - offset);
    SPECTRUM_ENFORCE_IF_NOT(buffer.size() == offset);
  }
  isMarkActive = true;
}

void RewindableImageSource::reset() {
  SPECTRUM_ENFORCE_IF_NOT(isMarkActive);
  isMarkActive = false;
  offset = buffer.size();
}

std::size_t RewindableImageSource::available() {
  return imageSource.available() + offset;
}

} // namespace io
} // namespace spectrum
} // namespace facebook
