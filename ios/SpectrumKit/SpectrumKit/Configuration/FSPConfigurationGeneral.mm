// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/Configuration.h>
#include <spectrum/core/SpectrumEnforce.h>

#import "FSPConfiguration.h"
#import "FSPConfiguration_Private.h"
#import "FSPPrivate.h"

using namespace facebook::spectrum;

@interface FSPConfigurationGeneral()

@property (nonatomic, assign) Configuration::General configuration;

@end

@implementation FSPConfigurationGeneral

- (instancetype)initWithDefaultBackgroundColor:(UIColor *)defaultBackgroundColor
                             interpretMetadata:(BOOL)interpretMetadata
         propagateChromaSamplingModeFromSource:(BOOL)propagateChromaSamplingModeFromSource
                    chromaSamplingModeOverride:(FSPConfigurationChromaSamplingModeOverride)chromaSamplingModeOverride
{
  if (self = [super init]) {
    self.defaultBackgroundColor = defaultBackgroundColor;
    self.interpretMetadata = interpretMetadata;
    self.propagateChromaSamplingModeFromSource = propagateChromaSamplingModeFromSource;
    self.chromaSamplingModeOverride = chromaSamplingModeOverride;
  }

  return self;
}

#pragma mark - Properties

- (UIColor *)defaultBackgroundColor
{
  const auto color = _configuration.defaultBackgroundColor();
  return [UIColor colorWithRed:static_cast<CGFloat>(color.red) / 255.0
                         green:static_cast<CGFloat>(color.green) / 255.0
                          blue:static_cast<CGFloat>(color.blue) / 255.0
                         alpha:1.0];
}

- (void)setDefaultBackgroundColor:(UIColor *)defaultBackgroundColor
{
  CGFloat red, green, blue, alpha;

  [defaultBackgroundColor getRed:&red green:&green blue:&blue alpha:&alpha];

  SPECTRUM_ENFORCE_IF_NOT(alpha == 1.0);

  _configuration.defaultBackgroundColor(image::Color{
    static_cast<uint8_t>(red * 255.0),
    static_cast<uint8_t>(green * 255.0),
    static_cast<uint8_t>(blue * 255.0)});
}

- (BOOL)interpretMetadata
{
  return _configuration.interpretMetadata();
}

- (void)setInterpretMetadata:(BOOL)interpretMetadata
{
  _configuration.interpretMetadata(interpretMetadata);
}

- (BOOL)propagateChromaSamplingModeFromSource
{
  return _configuration.propagateChromaSamplingModeFromSource();
}

- (void)setPropagateChromaSamplingModeFromSource:(BOOL)propagateChromaSamplingModeFromSource
{
  _configuration.propagateChromaSamplingModeFromSource(propagateChromaSamplingModeFromSource);
}

- (FSPConfigurationChromaSamplingModeOverride)chromaSamplingModeOverride
{
  return FSPConfigurationChromaSamplingModeOverrideFromInternalSamplingMode(_configuration.chromaSamplingModeOverride());
}

- (void)setChromaSamplingModeOverride:(FSPConfigurationChromaSamplingModeOverride)chromaSamplingModeOverride
{
  _configuration.chromaSamplingModeOverride(FSPInternalConfigurationChromaSamplingModeFromSamplingMode(chromaSamplingModeOverride));
}

#pragma mark - Equality

- (BOOL)isEqualToConfigurationGeneral:(FSPConfigurationGeneral *)object
{
  if (object == nil) {
    return NO;
  }

  return (FSPObjectEqualObject(self.defaultBackgroundColor, object.defaultBackgroundColor) &&
          self.interpretMetadata == object.interpretMetadata &&
          self.propagateChromaSamplingModeFromSource == object.propagateChromaSamplingModeFromSource &&
          self.chromaSamplingModeOverride == object.chromaSamplingModeOverride);
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPConfigurationGeneral class]]) {
    return [self isEqualToConfigurationGeneral:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return self.defaultBackgroundColor.hash ^ self.interpretMetadata ^ self.propagateChromaSamplingModeFromSource ^ self.chromaSamplingModeOverride;
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithDefaultBackgroundColor:self.defaultBackgroundColor
                                                         interpretMetadata:self.interpretMetadata
                                     propagateChromaSamplingModeFromSource:self.propagateChromaSamplingModeFromSource
                                                chromaSamplingModeOverride:self.chromaSamplingModeOverride];
}

#pragma mark - Internal

- (facebook::spectrum::Configuration::General)configuration
{
  return _configuration;
}

static FSPConfigurationChromaSamplingModeOverride FSPConfigurationChromaSamplingModeOverrideFromInternalSamplingMode(const Configuration::General::ChromaSamplingModeOverride& chromaSamplingModeOverride)
{
  switch (chromaSamplingModeOverride) {
    case Configuration::General::ChromaSamplingModeOverride::None:
      return FSPConfigurationChromaSamplingModeOverrideNone;
    case Configuration::General::ChromaSamplingModeOverride::S444:
      return FSPConfigurationChromaSamplingModeOverride444;
    case Configuration::General::ChromaSamplingModeOverride::S420:
      return FSPConfigurationChromaSamplingModeOverride420;
    case Configuration::General::ChromaSamplingModeOverride::S422:
      return FSPConfigurationChromaSamplingModeOverride422;
    case Configuration::General::ChromaSamplingModeOverride::S411:
      return FSPConfigurationChromaSamplingModeOverride411;
    case Configuration::General::ChromaSamplingModeOverride::S440:
      return FSPConfigurationChromaSamplingModeOverride440;
  }
}

static Configuration::General::ChromaSamplingModeOverride FSPInternalConfigurationChromaSamplingModeFromSamplingMode(const FSPConfigurationChromaSamplingModeOverride chromaSamplingModeOverride)
{
  switch (chromaSamplingModeOverride) {
    case FSPConfigurationChromaSamplingModeOverrideNone:
      return Configuration::General::ChromaSamplingModeOverride::None;
    case FSPConfigurationChromaSamplingModeOverride444:
      return Configuration::General::ChromaSamplingModeOverride::S444;
    case FSPConfigurationChromaSamplingModeOverride420:
      return Configuration::General::ChromaSamplingModeOverride::S420;
    case FSPConfigurationChromaSamplingModeOverride422:
      return Configuration::General::ChromaSamplingModeOverride::S422;
    case FSPConfigurationChromaSamplingModeOverride411:
      return Configuration::General::ChromaSamplingModeOverride::S411;
    case FSPConfigurationChromaSamplingModeOverride440:
      return Configuration::General::ChromaSamplingModeOverride::S440;
  }
}

@end
