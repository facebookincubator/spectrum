// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPImageFormat.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSInteger FSPEncodeRequirementQuality NS_SWIFT_NAME(EncodeRequirementQuality);

extern const FSPEncodeRequirementQuality FSPEncodeRequirementQualityMin NS_SWIFT_NAME(EncodeRequirementQualityMin);
extern const FSPEncodeRequirementQuality FSPEncodeRequirementQualityMax NS_SWIFT_NAME(EncodeRequirementQualityMax);

typedef NS_ENUM(NSUInteger, FSPEncodeRequirementMode) {
  FSPEncodeRequirementModeLossless,
  FSPEncodeRequirementModeLossy,
  FSPEncodeRequirementModeAny
} NS_SWIFT_NAME(EncodeRequirementMode);

NS_SWIFT_NAME(EncodeRequirement)
@interface FSPEncodeRequirement : NSObject <NSCopying>

+ (instancetype)new NS_UNAVAILABLE;

+ (instancetype)encodeRequirementWithFormat:(FSPEncodedImageFormat *)format;
+ (instancetype)encodeRequirementWithFormat:(FSPEncodedImageFormat *)format
                                       mode:(FSPEncodeRequirementMode)mode;
+ (instancetype)encodeRequirementWithFormat:(FSPEncodedImageFormat *)format
                                    quality:(FSPEncodeRequirementQuality)quality;
+ (instancetype)encodeRequirementWithFormat:(FSPEncodedImageFormat *)format
                                       mode:(FSPEncodeRequirementMode)mode
                                    quality:(FSPEncodeRequirementQuality)quality;

@property (nonatomic, strong, readonly) FSPEncodedImageFormat *format;
@property (nonatomic, assign, readonly) FSPEncodeRequirementMode mode;
@property (nonatomic, assign, readonly) FSPEncodeRequirementQuality quality;

- (instancetype)init NS_UNAVAILABLE;
- (BOOL)isEqualToEncodeRequirement:(nullable FSPEncodeRequirement *)object;

@end

NS_ASSUME_NONNULL_END
