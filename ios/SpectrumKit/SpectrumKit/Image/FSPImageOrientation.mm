// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPImageOrientation.h"
#import "FSPImageOrientation_Private.h"

using namespace facebook::spectrum;

FSPImageOrientation FSPImageOrientationFromImage(UIImage *image)
{
  switch (image.imageOrientation) {
    case UIImageOrientationUp:
      return FSPImageOrientationUp;
    case UIImageOrientationDown:
      return FSPImageOrientationBottom;
    case UIImageOrientationLeft:
      return FSPImageOrientationLeft;
    case UIImageOrientationRight:
      return FSPImageOrientationRight;
    case UIImageOrientationUpMirrored:
      return FSPImageOrientationUpMirrored;
    case UIImageOrientationDownMirrored:
      return FSPImageOrientationBottomMirrored;
    case UIImageOrientationLeftMirrored:
      return FSPImageOrientationLeftMirrored;
    case UIImageOrientationRightMirrored:
      return FSPImageOrientationRightMirrored;
  }
}

image::Orientation FSPInternalImageOrientationFromUIImageOrientation(const UIImageOrientation orientation)
{
  switch (orientation) {
    case UIImageOrientationUp:
      return image::Orientation::Up;
    case UIImageOrientationDown:
      return image::Orientation::Bottom;
    case UIImageOrientationLeft:
      return image::Orientation::Left;
    case UIImageOrientationRight:
      return image::Orientation::Right;
    case UIImageOrientationUpMirrored:
      return image::Orientation::UpMirrored;
    case UIImageOrientationDownMirrored:
      return image::Orientation::BottomMirrored;
    case UIImageOrientationLeftMirrored:
      return image::Orientation::LeftMirrored;
    case UIImageOrientationRightMirrored:
      return image::Orientation::RightMirrored;
  }
}

#pragma mark - Internal

FSPImageOrientation FSPImageOrientationFromInternalImageOrientation(const image::Orientation orientation)
{
  switch (orientation) {
    case image::Orientation::Up:
      return FSPImageOrientationUp;
    case image::Orientation::UpMirrored:
      return FSPImageOrientationUpMirrored;
    case image::Orientation::Bottom:
      return FSPImageOrientationBottom;
    case image::Orientation::BottomMirrored:
      return FSPImageOrientationBottomMirrored;
    case image::Orientation::LeftMirrored:
      return FSPImageOrientationLeftMirrored;
    case image::Orientation::Right:
      return FSPImageOrientationRight;
    case image::Orientation::RightMirrored:
      return FSPImageOrientationRightMirrored;
    case image::Orientation::Left:
      return FSPImageOrientationLeft;
  }
}

image::Orientation FSPInternalImageOrientationFromImageOrientation(const FSPImageOrientation orientation) {
  switch (orientation) {
    case FSPImageOrientationUp:
      return image::Orientation::Up;
    case FSPImageOrientationLeft:
      return image::Orientation::Left;
    case FSPImageOrientationBottom:
      return image::Orientation::Bottom;
    case FSPImageOrientationRight:
      return image::Orientation::Right;
    case FSPImageOrientationUpMirrored:
      return image::Orientation::UpMirrored;
    case FSPImageOrientationRightMirrored:
      return image::Orientation::RightMirrored;
    case FSPImageOrientationBottomMirrored:
      return image::Orientation::BottomMirrored;
    case FSPImageOrientationLeftMirrored:
      return image::Orientation::LeftMirrored;
  }
}
