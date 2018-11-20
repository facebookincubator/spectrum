// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPDefaultPlugin.h"

#import <SpectrumKit/FSPJpegPlugin.h>
#import <SpectrumKit/FSPPngPlugin.h>
#import <SpectrumKit/FSPWebpPlugin.h>

@implementation FSPSpectrum (DefaultPlugin)

+ (FSPSpectrum *)sharedInstance
{
  static dispatch_once_t onceToken;
  static FSPSpectrum *instance = nil;
  dispatch_once(&onceToken, ^{
    NSArray<id<FSPPlugin>> *plugins = @[[FSPJpegPlugin new], [FSPPngPlugin new], [FSPWebpPlugin new]];
    instance = [[FSPSpectrum alloc] initWithPlugins:plugins configuration:nil];
  });

  return instance;
}

@end
