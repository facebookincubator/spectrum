// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * Represents an image format. The sub-class `FSPEncodedFormat` indicates that the format is encoded.
 */
NS_SWIFT_NAME(ImageFormat)
@interface FSPImageFormat : NSObject <NSCopying>

+ (instancetype)new NS_UNAVAILABLE;
+ (instancetype)imageFormatWithIdentifier:(NSString *)identifier;

/** An uncompressed image of continious pixel data */
@property (class, nonatomic, readonly) FSPImageFormat *bitmap;

/**
 * Unique identifier of this format. E.g. "jpeg"
 */
@property (nonatomic, readonly) NSString *identifier;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithIdentifier:(NSString *)identifier NS_DESIGNATED_INITIALIZER;

- (BOOL)isEqualToImageFormat:(FSPImageFormat *)object;

@end

/**
 * Represents a compressed image format. See `ImageFormat` for uncompressed image formats.
 */
NS_SWIFT_NAME(EncodedImageFormat)
@interface FSPEncodedImageFormat : FSPImageFormat

/** A JPEG encoded image, see: https://en.wikipedia.org/wiki/JPEG */
@property (class, nonatomic, readonly) FSPEncodedImageFormat *jpeg;

/** A PNG encoded image, see https://en.wikipedia.org/wiki/Portable_Network_Graphics */
@property (class, nonatomic, readonly) FSPEncodedImageFormat *png;

/** A GIF encoded image, see https://en.wikipedia.org/wiki/GIF */
@property (class, nonatomic, readonly) FSPEncodedImageFormat *gif;

/** A WebP encoded image, see https://en.wikipedia.org/wiki/WebP */
@property (class, nonatomic, readonly) FSPEncodedImageFormat *webp;

/** A HEIF encoded image, see https://en.wikipedia.org/wiki/High_Efficiency_Image_File_Format */
@property (class, nonatomic, readonly) FSPEncodedImageFormat *heif;

@end

NS_ASSUME_NONNULL_END
