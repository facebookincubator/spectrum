// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPRotateRequirement.h"
#import "FSPRotateRequirement_Private.h"

#include <spectrum/core/utils/Numeric.h>

using namespace facebook::spectrum;

@implementation FSPRotateRequirement

+ (instancetype)rotateRequirementWithDegrees:(NSInteger)rotationDegrees
                      shouldFlipHorizontally:(BOOL)shouldFlipHorizontally
                        shouldFlipVertically:(BOOL)shouldFlipVertically
                    shouldForceUpOrientation:(BOOL)shouldForceUpOrientation
{
  return [[self alloc] initWithDegrees:rotationDegrees
                shouldFlipHorizontally:shouldFlipHorizontally
                  shouldFlipVertically:shouldFlipVertically
              shouldForceUpOrientation:shouldForceUpOrientation];
}

+ (instancetype)rotateRequirementWithDegrees:(NSInteger)rotationDegrees
{
  return [self rotateRequirementWithDegrees:rotationDegrees
                     shouldFlipHorizontally:NO
                       shouldFlipVertically:NO
                   shouldForceUpOrientation:NO];
}

+ (instancetype)rotateRequirementWithForceUpOrientation:(BOOL)forceUpOrientation
{
  return [self rotateRequirementWithDegrees:0
                     shouldFlipHorizontally:NO
                       shouldFlipVertically:NO
                   shouldForceUpOrientation:forceUpOrientation];
}

- (instancetype)initWithDegrees:(NSInteger)rotationDegrees
         shouldFlipHorizontally:(BOOL)shouldFlipHorizontally
           shouldFlipVertically:(BOOL)shouldFlipVertically
       shouldForceUpOrientation:(BOOL)shouldForceUpOrientation
{
  if (self = [super init]) {
    _degrees = rotationDegrees;
    _shouldFlipHorizontally = shouldFlipHorizontally;
    _shouldFlipVertically = shouldFlipVertically;
    _shouldForceUpOrientation = shouldForceUpOrientation;
  }

  return self;
}

#pragma mark - Equality

- (BOOL)isEqualToRotateRequirement:(FSPRotateRequirement *)object
{
  if (object == nil) {
    return NO;
  }

  return (core::numeric::angleDegreesSanitized(static_cast<int>(_degrees)) == core::numeric::angleDegreesSanitized(static_cast<int>(object.degrees)) &&
          _shouldFlipHorizontally == object.shouldFlipHorizontally &&
          _shouldFlipVertically == object.shouldFlipVertically &&
          _shouldForceUpOrientation == object.shouldForceUpOrientation);
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPRotateRequirement class]]) {
    return [self isEqualToRotateRequirement:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return _degrees ^ _shouldFlipHorizontally ^ _shouldFlipVertically ^ _shouldForceUpOrientation;
}

#pragma mark - Internal

- (requirements::Rotate)makeInternalRotateRequirement
{
  return requirements::Rotate {
    .degrees = SPECTRUM_CONVERT_OR_THROW(_degrees, int),
    .flipHorizontally = SPECTRUM_CONVERT_OR_THROW(_shouldFlipHorizontally, bool),
    .flipVertically = SPECTRUM_CONVERT_OR_THROW(_shouldFlipVertically, bool),
    .forceUpOrientation = SPECTRUM_CONVERT_OR_THROW(_shouldForceUpOrientation, bool),
  };
}

@end
