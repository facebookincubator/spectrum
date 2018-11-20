// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/IImageSource.h>

namespace facebook {
namespace spectrum {
namespace io {

/**
 * An image source that is backed by an encoded image.
 */
class IEncodedImageSource : public IImageSource {
 public:
  virtual ~IEncodedImageSource() = default;

 protected:
  IEncodedImageSource() = default;
  IEncodedImageSource(IEncodedImageSource&&) = default;
  IEncodedImageSource(const IEncodedImageSource&) = delete;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
