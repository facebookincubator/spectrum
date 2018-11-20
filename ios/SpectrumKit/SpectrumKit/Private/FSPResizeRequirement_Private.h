// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPResizeRequirement.h>
#include <spectrum/requirements/Resize.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPResizeRequirement (Private)

- (const facebook::spectrum::requirements::Resize)makeInternalResizeRequirement;

@end

NS_ASSUME_NONNULL_END
