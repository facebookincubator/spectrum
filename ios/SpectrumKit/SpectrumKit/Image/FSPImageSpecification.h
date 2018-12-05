// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPImageFormat.h>
#import <SpectrumKit/FSPImageOrientation.h>
#import <SpectrumKit/FSPImagePixelSpecification.h>
#import <SpectrumKit/FSPImageMetadata.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, FSPImageChromaSamplingMode) {
  FSPImageChromaSamplingModeNone = 0,
  FSPImageChromaSamplingMode444,
  FSPImageChromaSamplingMode420,
  FSPImageChromaSamplingMode422,
  FSPImageChromaSamplingMode411,
  FSPImageChromaSamplingMode440,
} NS_SWIFT_NAME(ImageChromaSamplingMode);

/**
 * An image's specification contains all derived and extracted information
 * besides the pixel content.
 */
NS_SWIFT_NAME(ImageSpecification)
@interface FSPImageSpecification : NSObject

+ (instancetype)new NS_UNAVAILABLE;

+ (instancetype)imageSpecificationWithSize:(CGSize)size
                                    format:(FSPImageFormat *)format
                        pixelSpecification:(FSPImagePixelSpecification *)pixelSpecification
                               orientation:(FSPImageOrientation)orientation
                        chromaSamplingMode:(FSPImageChromaSamplingMode)chromaSamplingMode
                                  metadata:(nullable FSPImageMetadata *)metadata;

@property (nonatomic, assign, readonly) CGSize size;
@property (nonatomic, strong, readonly) FSPImageFormat *format;
@property (nonatomic, strong, readonly) FSPImagePixelSpecification *pixelSpecification;
@property (nonatomic, assign, readonly) FSPImageOrientation orientation;
@property (nonatomic, assign, readonly) FSPImageChromaSamplingMode chromaSamplingMode;
@property (nonatomic, strong, readonly) FSPImageMetadata *metadata;

- (instancetype)init NS_UNAVAILABLE;
- (BOOL)isEqualToImageSpecification:(nullable FSPImageSpecification *)object;

@end

NS_ASSUME_NONNULL_END
