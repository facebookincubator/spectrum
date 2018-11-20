// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPCropRequirementProviding.h>

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(RelativeToOriginCropRequirementValues)
typedef struct {
  float top, left, bottom, right;
} FSPRelativeToOriginCropRequirementValues;

BOOL FSPRelativeToOriginCropRequirementValuesEqualToValues(FSPRelativeToOriginCropRequirementValues lhs, FSPRelativeToOriginCropRequirementValues rhs);

NS_SWIFT_NAME(RelativeToOriginCropRequirement)
@interface FSPRelativeToOriginCropRequirement : NSObject <FSPCropRequirementProviding>

+ (instancetype)new NS_UNAVAILABLE;
+ (instancetype)relativeToOriginCropRequirementWithValues:(FSPRelativeToOriginCropRequirementValues)values enforcement:(FSPCropRequirementEnforcement)enforcement;
+ (instancetype)relativeToOriginCropRequirementWithValues:(FSPRelativeToOriginCropRequirementValues)values;

@property (nonatomic, assign, readonly) FSPRelativeToOriginCropRequirementValues values;
@property (nonatomic, assign, readonly) FSPCropRequirementEnforcement enforcement;

- (instancetype)init NS_UNAVAILABLE;
- (BOOL)isEqualToRelativeToOriginCropRequirement:(nullable FSPRelativeToOriginCropRequirement *)object;

@end

NS_ASSUME_NONNULL_END
