// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPCropRequirementProviding.h>
#include <spectrum/requirements/Crop.h>

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(CropRequirementProviding_Private)
@protocol FSPCropRequirementProviding_Private <FSPCropRequirementProviding>

- (const facebook::spectrum::requirements::Crop)makeInternalCropRequirement;

@end

NS_ASSUME_NONNULL_END
