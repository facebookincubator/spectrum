// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPConfigurationWebp.h"

#import "FSPConfiguration_Private.h"
#import "FSPPrivate.h"

#import <spectrum/Configuration.h>

using namespace facebook::spectrum;

@interface FSPConfigurationWebp()

@property (nonatomic, assign) Configuration::Webp configuration;

@end

@implementation FSPConfigurationWebp

- (instancetype)initWithMethod:(NSInteger)method imageHint:(FSPConfigurationWebpImageHint)imageHint
{
  if (self = [super init]) {
    self.method = method;
    self.imageHint = imageHint;
  }

  return self;
}

#pragma mark - Properties

- (void)setMethod:(NSInteger)method
{
  _configuration.method(static_cast<int>(method));
}

- (NSInteger)method
{
  return _configuration.method();
}

- (void)setImageHint:(FSPConfigurationWebpImageHint)imageHint
{
  _configuration.imageHint(FSPInternalConfigurationWebpImageHintFromImageHint(imageHint));
}

- (FSPConfigurationWebpImageHint)imageHint
{
  return FSPConfigurationWebpImageHintFromInternalImageHint(_configuration.imageHint());
}

#pragma mark - Equality

- (BOOL)isEqualToConfigurationWebp:(FSPConfigurationWebp *)object
{
  if (object == nil) {
    return NO;
  }

  return self.method == object.method && self.imageHint == object.imageHint;
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPConfigurationWebp class]]) {
    return [self isEqualToConfigurationWebp:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return self.method ^ self.imageHint;
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithMethod:self.method imageHint:self.imageHint];
}

#pragma mark - Internal

- (facebook::spectrum::Configuration::Webp)configuration
{
  return _configuration;
}

static Configuration::Webp::ImageHint FSPInternalConfigurationWebpImageHintFromImageHint(const FSPConfigurationWebpImageHint imageHint)
{
  switch (imageHint) {
    case FSPConfigurationWebpImageHintDefault:
      return Configuration::Webp::ImageHint::Default;
    case FSPConfigurationWebpImageHintPicture:
      return Configuration::Webp::ImageHint::Picture;
    case FSPConfigurationWebpImageHintPhoto:
      return Configuration::Webp::ImageHint::Photo;
    case FSPConfigurationWebpImageHintGraph:
      return Configuration::Webp::ImageHint::Graph;
  }
}

static FSPConfigurationWebpImageHint FSPConfigurationWebpImageHintFromInternalImageHint(const Configuration::Webp::ImageHint imageHint)
{
  switch (imageHint) {
    case Configuration::Webp::ImageHint::Default:
      return FSPConfigurationWebpImageHintDefault;
    case Configuration::Webp::ImageHint::Picture:
      return FSPConfigurationWebpImageHintPicture;
    case Configuration::Webp::ImageHint::Photo:
      return FSPConfigurationWebpImageHintPhoto;
    case Configuration::Webp::ImageHint::Graph:
      return FSPConfigurationWebpImageHintGraph;
  }
}

@end
