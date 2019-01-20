// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPConfiguration.h"
#import "FSPConfiguration_Private.h"
#import "FSPPrivate.h"

#import <spectrum/Configuration.h>

using namespace facebook::spectrum;

@interface FSPConfigurationPng()

@property (nonatomic, assign) Configuration::Png configuration;

@end

@implementation FSPConfigurationPng

- (instancetype)initWithUseInterlacing:(BOOL)useInterlacing
{
  if (self = [super init]) {
    self.useInterlacing = useInterlacing;
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

#pragma mark - Equality

- (BOOL)isEqualToConfigurationPng:(FSPConfigurationPng *)object
{
  if (object == nil) {
    return NO;
  }

  return self.useInterlacing == object.useInterlacing;
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
  return [[[self class] allocWithZone:zone] initWithUseInterlacing:self.useInterlacing];
}

#pragma mark - Internal

- (facebook::spectrum::Configuration::Png)configuration
{
  return _configuration;
}

@end
