// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "VectorImageSource.h"

#include <spectrum/core/Constants.h>
#include <spectrum/core/SpectrumEnforce.h>
#include <spectrum/io/IImageSource.h>

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace facebook {
namespace spectrum {
namespace io {

template <class Interface>
VectorImageSource<Interface>::VectorImageSource(std::vector<char> data)
    : Interface(), _data(std::move(data)) {}

template <class Interface>
std::size_t VectorImageSource<Interface>::getTotalBytesRead() const {
  return _offset;
}

template <class Interface>
std::size_t VectorImageSource<Interface>::read(
    char* const destination,
    const std::size_t length) {
  const auto bytesLeft = _data.size() - _offset;
  const auto bytesToRead = std::min(length, bytesLeft);

  std::copy_n(_data.data() + _offset, bytesToRead, destination);
  _offset += bytesToRead;

  return bytesToRead;
}

template <class Interface>
std::size_t VectorImageSource<Interface>::available() {
  return _data.size() - _offset;
}

template class VectorImageSource<IBitmapImageSource>;
template class VectorImageSource<IEncodedImageSource>;

} // namespace io
} // namespace spectrum
} // namespace facebook
