// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIImage.h>

#include <spectrum/io/IBitmapImageSource.h>
#include <folly/Optional.h>

namespace facebook { namespace spectrum { namespace io {

class BitmapImageSource : public IBitmapImageSource {
public:
  BitmapImageSource(UIImage *image);
  BitmapImageSource(BitmapImageSource&&) = default;
  BitmapImageSource(const BitmapImageSource&) = delete;

  ~BitmapImageSource() override;

  image::Specification imageSpecification() const override;
  std::size_t read(char* const destination, const std::size_t length) override;
  std::size_t getTotalBytesRead() const override;
  std::size_t available() override;

private:
  UIImage *const _image;
  image::Specification _imageSpecification;

  const std::size_t _bytesPerRowWithoutPadding;
  const std::size_t _bytesPerRowPadding;

  CFDataRef _bitmapDataOfCurrentSubImage{nullptr};

  std::size_t _nextSubImageIndex{0};
  std::size_t _positionInCurrentSubImage{0};
  std::size_t _totalBytesRead{0};

  void _updateCurrentSubImage();
  void _clearCurrentSubImage();
};

}}} // namespace
