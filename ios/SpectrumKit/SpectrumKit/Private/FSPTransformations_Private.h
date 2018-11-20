// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPTransformations.h>

#include <spectrum/Transformations.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPTransformations (Private)

- (facebook::spectrum::Transformations)makeInternalTransformations;

@end

NS_ASSUME_NONNULL_END
