// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, FSPCropRequirementEnforcement) {
  FSPCropRequirementEnforcementMustBeExact,
  FSPCropRequirementEnforcementApproximate
} NS_SWIFT_NAME(CropRequirementEnforcement);

NS_SWIFT_NAME(CropRequirementProviding)
@protocol FSPCropRequirementProviding <NSObject>

@property (nonatomic, assign, readonly) FSPCropRequirementEnforcement enforcement;

@end

NS_ASSUME_NONNULL_END
