// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIColor.h>

#import <SpectrumKit/FSPConfigurationGeneral.h>
#import <SpectrumKit/FSPConfigurationJpeg.h>
#import <SpectrumKit/FSPConfigurationPng.h>
#import <SpectrumKit/FSPConfigurationWebp.h>

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(Configuration)
@interface FSPConfiguration : NSObject <NSCopying>

@property (nonatomic, readonly, strong) FSPConfigurationGeneral *general;
@property (nonatomic, readonly, strong) FSPConfigurationJpeg *jpeg;
@property (nonatomic, readonly, strong) FSPConfigurationPng *png;
@property (nonatomic, readonly, strong) FSPConfigurationWebp *webp;

- (instancetype)initWithGeneral:(FSPConfigurationGeneral *)general
                           jpeg:(FSPConfigurationJpeg *)jpeg
                            png:(FSPConfigurationPng *)png
                           webp:(FSPConfigurationWebp *)webp NS_DESIGNATED_INITIALIZER;

- (BOOL)isEqualToConfiguration:(FSPConfiguration *)object;

@end

NS_ASSUME_NONNULL_END
