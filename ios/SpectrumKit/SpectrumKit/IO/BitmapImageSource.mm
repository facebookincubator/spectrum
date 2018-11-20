// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "BitmapImageSource.h"

#import <CoreFoundation/CFData.h>
#import <CoreGraphics/CoreGraphics.h>

#include <spectrum/core/SpectrumEnforce.h>
#include <folly/Optional.h>

#import "FSPImageOrientation_Private.h"
#import "FSPImageSpecification_Private.h"

namespace facebook {
namespace spectrum {
namespace io {

constexpr CGFloat countOfLinesPerSubImage = 16;

BitmapImageSource::BitmapImageSource(UIImage *image)
  : _image{image},
    _imageSpecification{[[FSPImageSpecification imageSpecificationFromImage:image] makeInternalImageSpecification]},
    _bytesPerRowWithoutPadding{(CGImageGetBitsPerPixel(image.CGImage) * CGImageGetWidth(image.CGImage)) / 8},
    _bytesPerRowPadding(CGImageGetBytesPerRow(image.CGImage) - _bytesPerRowWithoutPadding) {
      SPECTRUM_ERROR_IF(CGImageGetBytesPerRow(image.CGImage) < _bytesPerRowWithoutPadding, error::ImageSourceFailure);
}

BitmapImageSource::~BitmapImageSource() {
  if (_bitmapDataOfCurrentSubImage != nullptr) {
    // Release in case not all reads have been performed.
    _clearCurrentSubImage();
  }
}

void BitmapImageSource::_updateCurrentSubImage() {
  const CGFloat height = std::min(
      countOfLinesPerSubImage,
      CGImageGetHeight(_image.CGImage) - (_nextSubImageIndex * countOfLinesPerSubImage));
  const CGRect rect = CGRectMake(
      0,
      _nextSubImageIndex * countOfLinesPerSubImage,
      CGImageGetWidth(_image.CGImage),
      height);
  CGImage *const subImage = CGImageCreateWithImageInRect(_image.CGImage, rect);

  SPECTRUM_ERROR_CSTR_IF(subImage == nullptr,
      error::ImageSourceFailure,
      "cgimage_create_with_image.CGImage_in_rect_failed");

  CGDataProvider *const dataProvider = CGImageGetDataProvider(subImage);

  SPECTRUM_ERROR_CSTR_IF(dataProvider == nullptr,
      error::ImageSourceFailure,
      "cgimage_get_data_provider_failed");

  _bitmapDataOfCurrentSubImage = CGDataProviderCopyData(dataProvider);

  CFRelease(subImage);

  _positionInCurrentSubImage = 0;
  ++_nextSubImageIndex;
}

image::Specification BitmapImageSource::imageSpecification() const {
  return _imageSpecification;
}

void BitmapImageSource::_clearCurrentSubImage() {
  CFRelease(_bitmapDataOfCurrentSubImage);
  _bitmapDataOfCurrentSubImage = nullptr;
}

std::size_t BitmapImageSource::read(char* const destination, const std::size_t length) {
  SPECTRUM_ERROR_FORMAT_IF_NOT(
      length == _bytesPerRowWithoutPadding,
      error::ImageSourceFailure,
      "len: %lu, _bytesPerRowWithoutPadding: %lu, _bytesPerRowPadding: %lu, _totalBytesRead: %lu",
      static_cast<unsigned long>(length),
      static_cast<unsigned long>(_bytesPerRowWithoutPadding),
      static_cast<unsigned long>(_bytesPerRowPadding),
      static_cast<unsigned long>(_totalBytesRead));

  if (_bitmapDataOfCurrentSubImage == nullptr) {
    _updateCurrentSubImage();
  }

  SPECTRUM_ENFORCE_IF_NOT(
    static_cast<std::size_t>(CFDataGetLength(_bitmapDataOfCurrentSubImage)) >=
    _positionInCurrentSubImage + length);

  std::memcpy(destination, CFDataGetBytePtr(_bitmapDataOfCurrentSubImage) + _positionInCurrentSubImage, length);

  _positionInCurrentSubImage += length + _bytesPerRowPadding;

  // The last row doesn't have optional padding - since we're always adding the padding above, let's check
  // for greater or equal to.
  if (_positionInCurrentSubImage >= static_cast<std::size_t>(CFDataGetLength(_bitmapDataOfCurrentSubImage))) {
    _clearCurrentSubImage();
  }

  _totalBytesRead += length;
  return length;
}

std::size_t BitmapImageSource::getTotalBytesRead() const {
  return _totalBytesRead;
}

std::size_t BitmapImageSource::available() {
  const auto totalReadableBytes = CGImageGetHeight(_image.CGImage) * _bytesPerRowWithoutPadding;
  return totalReadableBytes - _totalBytesRead;
}

}
}
}
