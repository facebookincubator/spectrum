// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPConfigurationJpeg.h"

#import "FSPPrivate.h"

#import <spectrum/Configuration.h>

using namespace facebook::spectrum;

@interface FSPConfigurationJpeg ()

@property (nonatomic, assign) Configuration::Jpeg configuration;

@end

@implementation FSPConfigurationJpeg

- (instancetype)initWithUseTrellis:(BOOL)useTrellis
                    useProgressive:(BOOL)useProgressive
                   useOptimizeScan:(BOOL)useOptimizeScan
         useCompatibleDCScanOption:(BOOL)useCompatibleDCScanOption
                 usePSNRQuantTable:(BOOL)usePSNRQuantTable
{
  if (self = [super init]) {
    self.useTrellis = useTrellis;
    self.useProgressive = useProgressive;
    self.useOptimizeScan = useOptimizeScan;
    self.useCompatibleDCScanOption = useCompatibleDCScanOption;
    self.usePSNRQuantTable = usePSNRQuantTable;
  }

  return self;
}

#pragma mark - Properties

- (void)setUseTrellis:(BOOL)useTrellis
{
  _configuration.useTrellis(useTrellis);
}

- (BOOL)useTrellis
{
  return _configuration.useTrellis();
}

- (void)setUseProgressive:(BOOL)useProgressive
{
  _configuration.useProgressive(useProgressive);
}

- (BOOL)useProgressive
{
  return _configuration.useProgressive();
}

- (void)setUseOptimizeScan:(BOOL)useOptimizeScan
{
  _configuration.useOptimizeScan(useOptimizeScan);
}

- (BOOL)useOptimizeScan
{
  return _configuration.useOptimizeScan();
}

- (void)setUseCompatibleDCScanOption:(BOOL)useCompatibleDCScanOption
{
  _configuration.useCompatibleDcScanOpt(useCompatibleDCScanOption);
}

- (BOOL)useCompatibleDCScanOption
{
  return _configuration.useCompatibleDcScanOpt();
}

- (BOOL)usePSNRQuantTable
{
  return _configuration.usePsnrQuantTable();
}

- (void)setUsePSNRQuantTable:(BOOL)usePSNRQuantTable
{
  _configuration.usePsnrQuantTable(usePSNRQuantTable);
}

#pragma mark - Equality

- (BOOL)isEqualToConfigurationJpeg:(FSPConfigurationJpeg *)object
{
  if (object == nil) {
    return NO;
  }

  return (self.useTrellis == object.useTrellis &&
          self.useProgressive == object.useProgressive &&
          self.useOptimizeScan == object.useOptimizeScan &&
          self.useCompatibleDCScanOption == object.useCompatibleDCScanOption &&
          self.usePSNRQuantTable == object.usePSNRQuantTable);
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPConfigurationJpeg class]]) {
    return [self isEqualToConfigurationJpeg:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return self.useTrellis ^ self.useProgressive ^ self.useOptimizeScan ^ self.useCompatibleDCScanOption ^ self.usePSNRQuantTable;
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithUseTrellis:self.useTrellis
                                                useProgressive:self.useProgressive
                                               useOptimizeScan:self.useOptimizeScan
                                     useCompatibleDCScanOption:self.useCompatibleDCScanOption
                                             usePSNRQuantTable:self.usePSNRQuantTable];
}

#pragma mark - Internal

- (facebook::spectrum::Configuration::Jpeg)configuration
{
  return _configuration;
}

@end
