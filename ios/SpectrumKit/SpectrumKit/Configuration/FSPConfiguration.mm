// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPConfiguration.h"
#import "FSPConfiguration_Private.h"

#import "FSPPrivate.h"

#include <spectrum/Configuration.h>

using namespace facebook::spectrum;

@implementation FSPConfiguration

- (instancetype)init
{
  return [self initWithGeneral:[FSPConfigurationGeneral new]
                          jpeg:[FSPConfigurationJpeg new]
                           png:[FSPConfigurationPng new]
                          webp:[FSPConfigurationWebp new]];
}

- (instancetype)initWithGeneral:(FSPConfigurationGeneral *)general
                           jpeg:(FSPConfigurationJpeg *)jpeg
                            png:(FSPConfigurationPng *)png
                           webp:(FSPConfigurationWebp *)webp
{
  if (self = [super init]) {
    _general = general;
    _jpeg = jpeg;
    _png = png;
    _webp = webp;
  }

  return self;
}

#pragma mark - Equality

- (BOOL)isEqualToConfiguration:(FSPConfiguration *)object
{
  if (object == nil) {
    return NO;
  }

  return (FSPObjectEqualObject(_general, object.general) &&
          FSPObjectEqualObject(_jpeg, object.jpeg) &&
          FSPObjectEqualObject(_png, object.png) &&
          FSPObjectEqualObject(_webp, object.webp));
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPConfiguration class]]) {
    return [self isEqualToConfiguration:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return _general.hash ^ _jpeg.hash ^ _png.hash ^ _webp.hash;
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithGeneral:[_general copy]
                                                       jpeg:[_jpeg copy]
                                                        png:[_png copy]
                                                       webp:[_webp copy]];
}

#pragma mark - Internal

- (facebook::spectrum::Configuration)makeInternalConfiguration
{
  return Configuration {
    .general = _general.configuration,
    .jpeg = _jpeg.configuration,
    .png = _png.configuration,
    .webp = _webp.configuration,
  };
}

@end
