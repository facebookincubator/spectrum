// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPImageSpecification.h>
#include <spectrum/image/Specification.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPImageSpecification (Private)

+ (instancetype)imageSpecificationFromInternalImageSpecification:(const facebook::spectrum::image::Specification &)imageSpecification;
- (facebook::spectrum::image::Specification)makeInternalImageSpecification;
+ (instancetype)imageSpecificationFromImage:(UIImage *)image;

@end

NS_ASSUME_NONNULL_END
