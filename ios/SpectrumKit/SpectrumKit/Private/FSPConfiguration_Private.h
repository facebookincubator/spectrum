// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPConfiguration.h>

#import <spectrum/Configuration.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPConfiguration (Private)

- (facebook::spectrum::Configuration)makeInternalConfiguration;

@end

@interface FSPConfigurationGeneral (Private)

@property (nonatomic, readonly, assign) facebook::spectrum::Configuration::General configuration;

@end

@interface FSPConfigurationJpeg (Private)

@property (nonatomic, readonly, assign) facebook::spectrum::Configuration::Jpeg configuration;

@end

@interface FSPConfigurationPng (Private)

@property (nonatomic, readonly, assign) facebook::spectrum::Configuration::Png configuration;

@end

@interface FSPConfigurationWebp (Private)

@property (nonatomic, readonly, assign) facebook::spectrum::Configuration::Webp configuration;

@end

NS_ASSUME_NONNULL_END
