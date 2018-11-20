// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPGeometry_Private.h"

using namespace facebook::spectrum;

#pragma mark - Image Dimension

folly::Optional<image::Size> FSPInternalImageSizeOptionalFromCGSize(const CGSize size) {
  if (CGSizeEqualToSize(size, CGSizeZero)) {
    return folly::none;
  } else {
    return FSPInternalImageSizeFromCGSize(size);
  }
}

image::Size FSPInternalImageSizeFromCGSize(CGSize size)
{
  return image::Size{
    .width = static_cast<std::uint32_t>(size.width),
    .height = static_cast<std::uint32_t>(size.height)
  };
}

CGSize FSPSizeFromInternalImageSize(const folly::Optional<image::Size> &imageSize)
{
  const auto nonNullImageSize = imageSize.value_or(image::sizeZero);
  return CGSizeMake(static_cast<CGFloat>(nonNullImageSize.width),
                    static_cast<CGFloat>(nonNullImageSize.height));
}

BOOL FSPCGSizeIsValid(CGSize size)
{
  return size.width > 0 && size.height > 0;
}
