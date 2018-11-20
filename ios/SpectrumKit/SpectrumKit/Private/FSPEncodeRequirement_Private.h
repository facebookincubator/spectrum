// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPEncodeRequirement.h>
#include <spectrum/requirements/Encode.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPEncodeRequirement (Private)

- (facebook::spectrum::requirements::Encode)makeInternalEncodeRequirement;

@end

NS_ASSUME_NONNULL_END
