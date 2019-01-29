// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPConfiguration.h"
#import "FSPConfiguration_Private.h"
#import "FSPPrivate.h"

#import <spectrum/Configuration.h>

#import "FSPLog.h"

using namespace facebook::spectrum;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
const NSInteger FSPPngCompressionLevelNone = Configuration::Png::CompressionLevelNone;
const NSInteger FSPPngCompressionLevelBestSpeed = Configuration::Png::CompressionLevelBestSpeed;
const NSInteger FSPPngCompressionLevelBestCompression = Configuration::Png::CompressionLevelBestCompression;
const NSInteger FSPPngCompressionLevelDefault = Configuration::Png::CompressionLevelDefault;
#pragma clang diagnostic pop

@interface FSPConfigurationPng()

@property (nonatomic, assign) Configuration::Png configuration;

@end

@implementation FSPConfigurationPng

- (instancetype)initWithUseInterlacing:(BOOL)useInterlacing
                      compressionLevel:(FSPPngCompressionLevel)compressionLevel
{
  FSPReportMustFixIf(compressionLevel < FSPPngCompressionLevelDefault, nil);
  FSPReportMustFixIf(compressionLevel > FSPPngCompressionLevelBestCompression, nil);

  if (self = [super init]) {
    self.useInterlacing = useInterlacing;
    self.compressionLevel = compressionLevel;
  }

  return self;
}

#pragma mark - Properties

- (void)setUseInterlacing:(BOOL)useInterlacing
{
  _configuration.useInterlacing(useInterlacing);
}

- (BOOL)useInterlacing
{
  return _configuration.useInterlacing();
}

- (void)setCompressionLevel:(FSPPngCompressionLevel)compressionLevel
{
  FSPReportMustFixIf(compressionLevel < FSPPngCompressionLevelDefault, nil);
  FSPReportMustFixIf(compressionLevel > FSPPngCompressionLevelBestCompression, nil);

  _configuration.compressionLevel(SPECTRUM_CONVERT_OR_THROW(compressionLevel, Configuration::Png::CompressionLevel));
}

- (FSPPngCompressionLevel)compressionLevel
{
  return _configuration.compressionLevel();
}

#pragma mark - Equality

- (BOOL)isEqualToConfigurationPng:(FSPConfigurationPng *)object
{
  if (object == nil) {
    return NO;
  }

  return self.useInterlacing == object.useInterlacing && self.compressionLevel == object.compressionLevel;
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPConfigurationPng class]]) {
    return [self isEqualToConfigurationPng:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return self.useInterlacing;
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithUseInterlacing:self.useInterlacing
                                                  compressionLevel:self.compressionLevel];
}

#pragma mark - Internal

- (facebook::spectrum::Configuration::Png)configuration
{
  return _configuration;
}

@end
