// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIColor.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSInteger FSPPngCompressionLevel NS_SWIFT_NAME(PngCompressionLevel);

extern const FSPPngCompressionLevel FSPPngCompressionLevelNone NS_SWIFT_NAME(PngCompressionLevelNone);
extern const FSPPngCompressionLevel FSPPngCompressionLevelBestSpeed NS_SWIFT_NAME(PngCompressionLevelBestSpeed);
extern const FSPPngCompressionLevel FSPPngCompressionLevelBestCompression NS_SWIFT_NAME(PngCompressionLevelBestCompression);
extern const FSPPngCompressionLevel FSPPngCompressionLevelDefault NS_SWIFT_NAME(PngCompressionLevelDefault);

NS_SWIFT_NAME(ConfigurationPng)
@interface FSPConfigurationPng : NSObject <NSCopying>

@property (nonatomic, assign) BOOL useInterlacing;
@property (nonatomic, assign) FSPPngCompressionLevel compressionLevel;

- (instancetype)initWithUseInterlacing:(BOOL)useInterlacing
                      compressionLevel:(FSPPngCompressionLevel)compressionLevel;

- (BOOL)isEqualToConfigurationPng:(FSPConfigurationPng *)object;

@end

NS_ASSUME_NONNULL_END
