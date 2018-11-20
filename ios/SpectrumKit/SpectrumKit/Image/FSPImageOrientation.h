// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIImage.h>

/**
 * The orientation describes the transformation that needs to be applied to the
 * underlying image in order to reach its intended representation.
 */
typedef NS_ENUM(NSUInteger, FSPImageOrientation) {
  FSPImageOrientationUp,
  FSPImageOrientationLeft,
  FSPImageOrientationBottom,
  FSPImageOrientationRight,
  FSPImageOrientationUpMirrored,
  FSPImageOrientationRightMirrored,
  FSPImageOrientationBottomMirrored,
  FSPImageOrientationLeftMirrored,
} NS_SWIFT_NAME(ImageOrientation);

FSPImageOrientation FSPImageOrientationFromImage(UIImage *image);
