// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/IImageSink.h>

namespace facebook {
namespace spectrum {
namespace io {

/**
 * An image sink that is backed by a bitmap.
 */
class IBitmapImageSink : public IImageSink {
 public:
  virtual ~IBitmapImageSink() = default;

 protected:
  IBitmapImageSink() = default;
  IBitmapImageSink(IBitmapImageSink&&) = default;
  IBitmapImageSink(const IBitmapImageSink&) = delete;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
