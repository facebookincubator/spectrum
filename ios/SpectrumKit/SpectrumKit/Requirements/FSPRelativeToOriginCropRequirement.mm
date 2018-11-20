// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPRelativeToOriginCropRequirement.h"

#import "FSPLog.h"
#include <spectrum/requirements/CropRelativeToOrigin.h>

#import "FSPCropRequirementProviding_Private.h"

using namespace facebook::spectrum;

@interface FSPRelativeToOriginCropRequirement () <FSPCropRequirementProviding_Private>
@end

@implementation FSPRelativeToOriginCropRequirement

#pragma mark - Convenience Initializers

+ (instancetype)relativeToOriginCropRequirementWithValues:(FSPRelativeToOriginCropRequirementValues)values
{
  return [self relativeToOriginCropRequirementWithValues:values enforcement:FSPCropRequirementEnforcementMustBeExact];
}

+ (instancetype)relativeToOriginCropRequirementWithValues:(FSPRelativeToOriginCropRequirementValues)values enforcement:(FSPCropRequirementEnforcement)enforcement
{
  return [[self alloc] initWithValues:values enforcement:enforcement];
}

#pragma mark - Initializers

- (instancetype)initWithValues:(FSPRelativeToOriginCropRequirementValues)values enforcement:(FSPCropRequirementEnforcement)enforcement
{
  FSPReportMustFixIfNot(FSPRelativeToOriginCropRequirementValidValues(values), nil);

  if (self = [super init]) {
    _values = values;
    _enforcement = enforcement;
  }

  return self;
}

#pragma mark - Equality

- (BOOL)isEqualToRelativeToOriginCropRequirement:(FSPRelativeToOriginCropRequirement *)object
{
  if (object == nil) {
    return NO;
  }

  return (_enforcement == object.enforcement && FSPRelativeToOriginCropRequirementValuesEqualToValues(_values, object.values));
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPRelativeToOriginCropRequirement class]]) {
    return [self isEqualToRelativeToOriginCropRequirement:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return _enforcement ^ FSPRelativeCropRequirementValuesHash(_values);
}

#pragma mark - FSPCropRequirementProviding_Private

- (requirements::Crop)makeInternalCropRequirement
{
  return requirements::CropRelativeToOrigin({
    .left = _values.left, .top = _values.top,
    .right = _values.right, .bottom = _values.bottom
  }, _enforcement == FSPCropRequirementEnforcementMustBeExact);
}

#pragma mark - Helpers

BOOL FSPRelativeToOriginCropRequirementValuesEqualToValues(FSPRelativeToOriginCropRequirementValues lhs, FSPRelativeToOriginCropRequirementValues rhs)
{
  return lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom;
}

static BOOL FSPRelativeToOriginCropRequirementValidValues(FSPRelativeToOriginCropRequirementValues values)
{
  // 0.0f <= left < right <= 1.0f
  const BOOL isHorizontalAxisValid = values.left >= 0.0f && values.left < values.right && values.right <= 1.0f;
  // 0.0f <= top < bottom <= 1.0f
  const BOOL isVerticalAxisValid = values.top >= 0.0f && values.top < values.bottom && values.bottom <= 1.0f;
  return isHorizontalAxisValid && isVerticalAxisValid;
}

static NSUInteger FSPRelativeCropRequirementValuesHash(FSPRelativeToOriginCropRequirementValues object)
{
  return @(object.left).hash ^ @(object.top).hash ^ @(object.right).hash ^ @(object.bottom).hash;
}

@end
