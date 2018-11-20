// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPImageOrientation.h>
#include <spectrum/image/Orientation.h>

NS_ASSUME_NONNULL_BEGIN

FSPImageOrientation FSPImageOrientationFromInternalImageOrientation(const facebook::spectrum::image::Orientation orientation);
facebook::spectrum::image::Orientation FSPInternalImageOrientationFromImageOrientation(const FSPImageOrientation orientation);
facebook::spectrum::image::Orientation FSPInternalImageOrientationFromUIImageOrientation(const UIImageOrientation orientation);

NS_ASSUME_NONNULL_END
