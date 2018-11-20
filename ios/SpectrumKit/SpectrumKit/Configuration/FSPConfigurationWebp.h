// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIColor.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, FSPConfigurationWebpImageHint) {
  FSPConfigurationWebpImageHintDefault,
  FSPConfigurationWebpImageHintPicture,
  FSPConfigurationWebpImageHintPhoto,
  FSPConfigurationWebpImageHintGraph,
} NS_SWIFT_NAME(ConfigurationWebpImageHint);

NS_SWIFT_NAME(ConfigurationWebp)
@interface FSPConfigurationWebp : NSObject <NSCopying>

@property (nonatomic, assign) NSInteger method;
@property (nonatomic, assign) FSPConfigurationWebpImageHint imageHint;

- (instancetype)initWithMethod:(NSInteger)method
                     imageHint:(FSPConfigurationWebpImageHint)imageHint;

- (BOOL)isEqualToConfigurationWebp:(FSPConfigurationWebp *)object;

@end

NS_ASSUME_NONNULL_END
