// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "InputStreamImageSource.h"

#include <spectrum/core/SpectrumEnforce.h>

#import "NSError+Spectrum_Private.h"

namespace facebook { namespace spectrum { namespace io {

InputStreamImageSource::InputStreamImageSource(NSInputStream *inputStream)
  : _inputStream(inputStream) {
    [_inputStream open];

    SPECTRUM_ERROR_STRING_IF(
        _inputStream.streamError != nil,
        error::ImageSourceFailure,
        FSPMakeMessageFromError(_inputStream.streamError));
}

InputStreamImageSource::~InputStreamImageSource() {
  [_inputStream close];
}

std::size_t InputStreamImageSource::read(char * const destination, const std::size_t length) {
  NSInteger bytesRead = [_inputStream read:(std::uint8_t *)destination maxLength:length];

  SPECTRUM_ERROR_STRING_IF(bytesRead < 0,
      error::ImageSourceFailure,
      FSPMakeMessageFromError(_inputStream.streamError));

  _totalBytesRead += bytesRead;

  return (std::size_t)bytesRead;
}

std::size_t InputStreamImageSource::getTotalBytesRead() const {
  return _totalBytesRead;
}

std::size_t InputStreamImageSource::available() {
  return [_inputStream hasBytesAvailable ] ? 1 : 0;
}

}}}
