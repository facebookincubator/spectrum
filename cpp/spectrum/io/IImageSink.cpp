// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "IImageSink.h"

namespace facebook {
namespace spectrum {
namespace io {

namespace error {
const folly::StringPiece ImageSinkFailure{"image_sink_failure"};
}

void IImageSink::write(const char* src, const std::size_t len) {
  _write(src, len);
  _totalBytesWritten += len;
}

std::size_t IImageSink::totalBytesWritten() const {
  return _totalBytesWritten;
}

} // namespace io
} // namespace spectrum
} // namespace facebook
