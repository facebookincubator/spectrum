// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPAbsoluteToOriginCropRequirement.h"

#include <spectrum/requirements/CropAbsoluteToOrigin.h>

#import "FSPCropRequirementProviding_Private.h"

#import "FSPLog.h"

using namespace facebook::spectrum;

@interface FSPAbsoluteToOriginCropRequirement () <FSPCropRequirementProviding_Private>
@end

@implementation FSPAbsoluteToOriginCropRequirement

#pragma mark - Convenience Initializers

+ (instancetype)absoluteToOriginCropRequirementWithValues:(FSPAbsoluteToOriginCropRequirementValues)values
{
  return [self absoluteToOriginCropRequirementWithValues:values enforcement:FSPCropRequirementEnforcementMustBeExact];
}

+ (instancetype)absoluteToOriginCropRequirementWithValues:(FSPAbsoluteToOriginCropRequirementValues)values enforcement:(FSPCropRequirementEnforcement)enforcement
{
  return [[self alloc] initWithInsets:values enforcement:enforcement];
}

#pragma mark - Initializers

- (instancetype)initWithInsets:(FSPAbsoluteToOriginCropRequirementValues)values enforcement:(FSPCropRequirementEnforcement)enforcement
{
  FSPReportMustFixIfNot(FSPAbsoluteToOriginCropRequirementValidValues(values), nil);

  if (self = [super init]) {
    _values = values;
    _enforcement = enforcement;
  }

  return self;
}

#pragma mark - Equality

- (BOOL)isEqualToAbsoluteToOriginCropRequirement:(FSPAbsoluteToOriginCropRequirement *)object
{
  if (object == nil) {
    return NO;
  }

  return (_enforcement == object.enforcement && FSPAbsoluteToOriginCropRequirementValuesEqualToValues(_values, object.values));
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPAbsoluteToOriginCropRequirement class]]) {
    return [self isEqualToAbsoluteToOriginCropRequirement:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return _enforcement ^ FSPAbsoluteCropRequirementValuesHash(_values);
}

#pragma mark - FSPCropRequirementProviding_Private

- (requirements::Crop)makeInternalCropRequirement
{
  return requirements::CropAbsoluteToOrigin({
    .left = _values.left, .top = _values.top,
    .right = _values.right, .bottom = _values.bottom
  }, _enforcement == FSPCropRequirementEnforcementMustBeExact);
}

#pragma mark - Helpers

BOOL FSPAbsoluteToOriginCropRequirementValuesEqualToValues(FSPAbsoluteToOriginCropRequirementValues lhs, FSPAbsoluteToOriginCropRequirementValues rhs)
{
  return lhs.left == rhs.left && lhs.top == rhs.top && lhs.right == rhs.right && lhs.bottom == rhs.bottom;
}

static BOOL FSPAbsoluteToOriginCropRequirementValidValues(FSPAbsoluteToOriginCropRequirementValues values)
{
  // 0.0f <= left < right
  const BOOL isHorizontalAxisValid = values.left < values.right;
  // 0.0f <= top < bottom
  const BOOL isVerticalAxisValid = values.top < values.bottom;
  return isHorizontalAxisValid && isVerticalAxisValid;
}

static NSUInteger FSPAbsoluteCropRequirementValuesHash(FSPAbsoluteToOriginCropRequirementValues object)
{
  return @(object.left).hash ^ @(object.top).hash ^ @(object.right).hash ^ @(object.bottom).hash;
}

@end
