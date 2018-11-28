// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIColor.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, FSPConfigurationChromaSamplingModeOverride) {
  FSPConfigurationChromaSamplingModeOverrideNone = 0,
  FSPConfigurationChromaSamplingModeOverride444,
  FSPConfigurationChromaSamplingModeOverride420,
  FSPConfigurationChromaSamplingModeOverride422,
  FSPConfigurationChromaSamplingModeOverride411,
  FSPConfigurationChromaSamplingModeOverride440,
} NS_SWIFT_NAME(ConfigurationChromaSamplingMode);

NS_SWIFT_NAME(ConfigurationGeneral)
@interface FSPConfigurationGeneral : NSObject <NSCopying>

@property (nonatomic, strong) UIColor *defaultBackgroundColor;
@property (nonatomic, assign) BOOL interpretMetadata;
@property (nonatomic, assign) BOOL propagateChromaSamplingModeFromSource;
@property (nonatomic, assign) FSPConfigurationChromaSamplingModeOverride chromaSamplingModeOverride;

- (instancetype)initWithDefaultBackgroundColor:(UIColor *)defaultBackgroundColor
                             interpretMetadata:(BOOL)interpretMetadata
         propagateChromaSamplingModeFromSource:(BOOL)propagateChromaSamplingModeFromSource
                    chromaSamplingModeOverride:(FSPConfigurationChromaSamplingModeOverride)chromaSamplingModeOverride;

- (BOOL)isEqualToConfigurationGeneral:(FSPConfigurationGeneral *)object;

@end

NS_ASSUME_NONNULL_END
