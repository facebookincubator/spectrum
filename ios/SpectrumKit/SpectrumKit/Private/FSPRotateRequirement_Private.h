// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPRotateRequirement.h>
#include <spectrum/requirements/Rotate.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPRotateRequirement (Private)

- (facebook::spectrum::requirements::Rotate)makeInternalRotateRequirement;

@end

NS_ASSUME_NONNULL_END
