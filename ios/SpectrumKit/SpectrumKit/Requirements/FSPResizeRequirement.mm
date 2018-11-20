// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPResizeRequirement.h"
#import "FSPResizeRequirement_Private.h"

#import "FSPGeometry_Private.h"

using namespace facebook::spectrum;

@implementation FSPResizeRequirement

#pragma mark - Convenience Initializers

+ (instancetype)resizeRequirementExactWithTargetSize:(CGSize)targetSize
{
  return [[self alloc] initWithMode:FSPResizeRequirementModeExact targetSize:targetSize];
}

#pragma mark - Initialisers

- (instancetype)initWithMode:(FSPResizeRequirementMode)mode targetSize:(CGSize)targetSize
{
  if (self = [super init]) {
    _mode = mode;
    _targetSize = targetSize;
  }

  return self;
}

#pragma mark - Equality

- (BOOL)isEqualToResizeRequirement:(FSPResizeRequirement *)object
{
  if (object == nil) {
    return NO;
  }

  return _mode == object.mode && CGSizeEqualToSize(_targetSize, object.targetSize);
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPResizeRequirement class]]) {
    return [self isEqualToResizeRequirement:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return _mode ^ @(_targetSize.width).hash ^ @(_targetSize.height).hash;
}

#pragma mark - Internal

- (const facebook::spectrum::requirements::Resize)makeInternalResizeRequirement
{
  return requirements::Resize {
    .mode = FSPInternalResizeRequirementModeFromMode(_mode),
    .targetSize = FSPInternalImageSizeFromCGSize(_targetSize)
  };
}

static requirements::Resize::Mode FSPInternalResizeRequirementModeFromMode(const FSPResizeRequirementMode mode) {
  switch (mode) {
    case FSPResizeRequirementModeExactOrSmaller:
      return requirements::Resize::Mode::ExactOrSmaller;
    case FSPResizeRequirementModeExact:
      return requirements::Resize::Mode::Exact;
    case FSPResizeRequirementModeExactOrLarger:
      return requirements::Resize::Mode::ExactOrLarger;
  }
}

@end
