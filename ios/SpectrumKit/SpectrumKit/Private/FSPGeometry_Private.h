// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

#include <spectrum/image/Geometry.h>

NS_ASSUME_NONNULL_BEGIN

facebook::spectrum::image::Size FSPInternalImageSizeFromCGSize(CGSize size);
folly::Optional<facebook::spectrum::image::Size> FSPInternalImageSizeOptionalFromCGSize(const CGSize size);
CGSize FSPSizeFromInternalImageSize(const folly::Optional<facebook::spectrum::image::Size> &imageSize);

BOOL FSPCGSizeIsValid(CGSize size);

NS_ASSUME_NONNULL_END
