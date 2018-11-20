// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPCropRequirementProviding.h>

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(AbsoluteToOriginCropRequirementValues)
typedef struct {
  uint32_t top, left, bottom, right;
} FSPAbsoluteToOriginCropRequirementValues;

BOOL FSPAbsoluteToOriginCropRequirementValuesEqualToValues(FSPAbsoluteToOriginCropRequirementValues lhs, FSPAbsoluteToOriginCropRequirementValues rhs);

NS_SWIFT_NAME(AbsoluteToOriginCropRequirement)
@interface FSPAbsoluteToOriginCropRequirement : NSObject <FSPCropRequirementProviding>

+ (instancetype)new NS_UNAVAILABLE;
+ (instancetype)absoluteToOriginCropRequirementWithValues:(FSPAbsoluteToOriginCropRequirementValues)values enforcement:(FSPCropRequirementEnforcement)enforcement;
+ (instancetype)absoluteToOriginCropRequirementWithValues:(FSPAbsoluteToOriginCropRequirementValues)values;

@property (nonatomic, assign, readonly) FSPAbsoluteToOriginCropRequirementValues values;
@property (nonatomic, assign, readonly) FSPCropRequirementEnforcement enforcement;

- (instancetype)init NS_UNAVAILABLE;
- (BOOL)isEqualToAbsoluteToOriginCropRequirement:(nullable FSPAbsoluteToOriginCropRequirement *)object;

@end

NS_ASSUME_NONNULL_END
