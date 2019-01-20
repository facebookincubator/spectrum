// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIColor.h>

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(ConfigurationPng)
@interface FSPConfigurationPng : NSObject <NSCopying>

@property (nonatomic, assign) BOOL useInterlacing;

- (instancetype)initWithUseInterlacing:(BOOL)useInterlacing;

- (BOOL)isEqualToConfigurationPng:(FSPConfigurationPng *)object;

@end

NS_ASSUME_NONNULL_END
