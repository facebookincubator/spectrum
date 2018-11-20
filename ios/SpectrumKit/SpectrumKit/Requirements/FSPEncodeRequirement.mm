// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPEncodeRequirement.h"
#import "FSPEncodeRequirement_Private.h"

#import "FSPLog.h"
#include <spectrum/core/utils/Numeric.h>

#import "FSPImageFormat_Private.h"
#import "FSPPrivate.h"

using namespace facebook::spectrum;

const NSInteger FSPEncodeRequirementQualityDefault = requirements::Encode::QualityDefault;
const NSInteger FSPEncodeRequirementQualityMin = requirements::Encode::QualityMin;
const NSInteger FSPEncodeRequirementQualityMax = requirements::Encode::QualityMax;

@implementation FSPEncodeRequirement

#pragma mark - Convenience Initializers

+ (instancetype)encodeRequirementWithFormat:(FSPEncodedImageFormat *)format
{
  return [self encodeRequirementWithFormat:format
                                      mode:FSPEncodeRequirementModeAny
                                   quality:FSPEncodeRequirementQualityDefault];
}

+ (instancetype)encodeRequirementWithFormat:(FSPEncodedImageFormat *)format
                                    quality:(FSPEncodeRequirementQuality)quality
{
  return [self encodeRequirementWithFormat:format
                                      mode:FSPEncodeRequirementModeAny
                                   quality:quality];
}

+ (instancetype)encodeRequirementWithFormat:(FSPEncodedImageFormat *)format
                                       mode:(FSPEncodeRequirementMode)mode
{
  return [self encodeRequirementWithFormat:format
                                      mode:mode
                                   quality:FSPEncodeRequirementQualityDefault];
}

+ (instancetype)encodeRequirementWithFormat:(FSPEncodedImageFormat *)format
                                       mode:(FSPEncodeRequirementMode)mode
                                    quality:(FSPEncodeRequirementQuality)quality
{
  return [[self alloc] initWithFormat:format
                                 mode:mode
                              quality:quality];
}

#pragma mark - Initialisers

- (instancetype)initWithFormat:(FSPEncodedImageFormat *)format
                          mode:(FSPEncodeRequirementMode)mode
                       quality:(FSPEncodeRequirementQuality)quality
{
  FSPReportMustFixIfNil(format, nil);
  FSPReportMustFixIf(mode == FSPEncodeRequirementModeLossy && quality == 0, nil);
  FSPReportMustFixIf(quality > FSPEncodeRequirementQualityMax, nil);

  if (self = [super init]) {
    _format = format;
    _mode = mode;
    _quality = quality;
  }

  return self;
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithFormat:_format mode:_mode quality:_quality];
}

#pragma mark - Equality

- (BOOL)isEqualToEncodeRequirement:(FSPEncodeRequirement *)object
{
  if (object == nil) {
    return NO;
  }

  return FSPObjectEqualObject(_format, object.format) && _mode == object.mode && _quality == object.quality;
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPEncodeRequirement class]]) {
    return [self isEqualToEncodeRequirement:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return static_cast<NSUInteger>(_mode) ^ static_cast<NSUInteger>(_quality) ^ _format.hash;
}

#pragma mark - Internal

- (requirements::Encode)makeInternalEncodeRequirement
{
  return requirements::Encode {
    .quality = SPECTRUM_CONVERT_OR_THROW(_quality, requirements::Encode::Quality),
    .format = [_format makeInternalFormat],
    .mode = FSPInternalEncodeRequirementModeFromEncodeRequirementMode(_mode),
  };
}

#pragma mark - Internal

static requirements::Encode::Mode FSPInternalEncodeRequirementModeFromEncodeRequirementMode(const FSPEncodeRequirementMode mode)
{
  switch (mode) {
    case FSPEncodeRequirementModeAny:
      return requirements::Encode::Mode::Any;
    case FSPEncodeRequirementModeLossy:
      return requirements::Encode::Mode::Lossy;
    case FSPEncodeRequirementModeLossless:
      return requirements::Encode::Mode::Lossless;
  }
}

@end
