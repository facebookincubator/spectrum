// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "OutputStreamImageSink.h"

#include <spectrum/core/SpectrumEnforce.h>

#import "NSError+Spectrum_Private.h"

namespace facebook { namespace spectrum { namespace io {

OutputStreamImageSink::OutputStreamImageSink(NSOutputStream *outputStream)
  : IEncodedImageSink(), _outputStream(outputStream) {
    [_outputStream open];

    SPECTRUM_ERROR_STRING_IF(
        _outputStream.streamError != nil,
        error::ImageSinkFailure,
        FSPMakeMessageFromError(_outputStream.streamError));
}

OutputStreamImageSink::~OutputStreamImageSink() {
  [_outputStream close];
}

void OutputStreamImageSink::setConfiguration(
  __unused const image::Size& imageSize,
  __unused const image::pixel::Specification& pixelSpecification) {
  // files do not get size / pixel type
}

void OutputStreamImageSink::_write(const char *const source, const std::size_t length) {
  NSInteger writtenLen = [_outputStream write:(std::uint8_t *)source maxLength:length];

  SPECTRUM_ERROR_STRING_IF(writtenLen < 0,
      error::ImageSinkFailure,
      FSPMakeMessageFromError(_outputStream.streamError));
}

}}}
