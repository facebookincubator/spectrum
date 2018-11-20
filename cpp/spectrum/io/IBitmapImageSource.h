// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Specification.h>
#include <spectrum/io/IImageSource.h>

namespace facebook {
namespace spectrum {
namespace io {

/**
 * An image source that is backed by a bitmap.
 */
class IBitmapImageSource : public IImageSource {
 public:
  virtual ~IBitmapImageSource() = default;

  /**
   * Provides the specification of the underlying bitmap. Subclasses must return
   * a specification that matches the data returned from the read(...) method.
   */
  virtual image::Specification imageSpecification() const = 0;

 protected:
  IBitmapImageSource() = default;
  IBitmapImageSource(IBitmapImageSource&&) = default;
  IBitmapImageSource(const IBitmapImageSource&) = delete;
};

} // namespace io
} // namespace spectrum
} // namespace facebook
