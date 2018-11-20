// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPTransformations.h"
#import "FSPTransformations_Private.h"

#import "FSPCropRequirementProviding_Private.h"
#import "FSPResizeRequirement_Private.h"
#import "FSPRotateRequirement_Private.h"

#import "FSPPrivate.h"

#import "FSPLog.h"

using namespace facebook::spectrum;

@implementation FSPTransformations

#pragma mark - Initialisers

- (instancetype)initWithResizeRequirement:(nullable FSPResizeRequirement *)resizeRequirement
                        rotateRequirement:(nullable FSPRotateRequirement *)rotateRequirement
                          cropRequirement:(nullable NSObject<FSPCropRequirementProviding> *)cropRequirement
{
  if (self = [super init]) {
    _resizeRequirement = resizeRequirement;
    _rotateRequirement = rotateRequirement;
    _cropRequirement = cropRequirement;
  }

  return self;
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithResizeRequirement:_resizeRequirement
                                                    rotateRequirement:_rotateRequirement
                                                      cropRequirement:_cropRequirement];
}

#pragma mark - Equality

- (BOOL)isEqualToTransformations:(nullable FSPTransformations *)object
{
  return (FSPObjectEqualObject(_rotateRequirement, object.rotateRequirement) &&
          FSPObjectEqualObject(_resizeRequirement, object.resizeRequirement) &&
          FSPObjectEqualObject(_cropRequirement, object.cropRequirement));
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPTransformations class]]) {
    return [self isEqualToTransformations:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return (_rotateRequirement.hash ^ _resizeRequirement.hash ^ _cropRequirement.hash);
}

#pragma mark - Internal

- (Transformations)makeInternalTransformations
{
  auto transformations = Transformations{};

  if (_rotateRequirement != nil) {
    transformations.rotateRequirement = [_rotateRequirement makeInternalRotateRequirement];
  }

  if (_resizeRequirement != nil) {
    transformations.resizeRequirement = [_resizeRequirement makeInternalResizeRequirement];
  }

  if (_cropRequirement != nil) {
    transformations.cropRequirement = [(id<FSPCropRequirementProviding_Private>)_cropRequirement makeInternalCropRequirement];
  }

  return transformations;
}

@end
