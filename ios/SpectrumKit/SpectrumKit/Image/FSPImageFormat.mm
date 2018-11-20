// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPImageFormat.h"
#import "FSPImageFormat_Private.h"

#include <folly/FixedString.h>
#include <folly/Optional.h>

#import "FSPPrivate.h"

using namespace facebook::spectrum;

@implementation FSPImageFormat

+ (instancetype)imageFormatWithIdentifier:(NSString *)identifier
{
  return [[self alloc] initWithIdentifier:identifier];
}

+ (FSPImageFormat *)bitmap
{
  return [self imageFormatWithIdentifier:FSPStringFromInternalFixedString(image::formats::Bitmap.identifier())];
}

- (instancetype)initWithIdentifier:(NSString *)identifier
{
  if (self = [super init]) {
    _identifier = [identifier copy];
  }

  return self;
}

#pragma mark - Equality

- (BOOL)isEqualToImageFormat:(FSPImageFormat *)object
{
  if (object == nil) {
    return NO;
  }

  return FSPObjectEqualObject(_identifier, object.identifier);
}

#pragma mark - NSCopying

- (id)copyWithZone:(__unused NSZone *)zone
{
  return [[[self class] allocWithZone:zone] initWithIdentifier:_identifier];
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPImageFormat class]]) {
    return [self isEqualToImageFormat:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return _identifier.hash;
}

#pragma mark - Internal

- (image::Format)makeInternalFormat
{
  return image::Format{FSPInternalFixedStringFromString(_identifier)};
}

@end

@implementation FSPEncodedImageFormat

+ (FSPEncodedImageFormat *)jpeg
{
  return [self imageFormatWithIdentifier:FSPStringFromInternalFixedString(image::formats::Jpeg.identifier())];
}

+ (FSPEncodedImageFormat *)png
{
  return [self imageFormatWithIdentifier:FSPStringFromInternalFixedString(image::formats::Png.identifier())];
}

+ (FSPEncodedImageFormat *)gif
{
  return [self imageFormatWithIdentifier:FSPStringFromInternalFixedString(image::formats::Gif.identifier())];
}

+ (FSPEncodedImageFormat *)webp
{
  return [self imageFormatWithIdentifier:FSPStringFromInternalFixedString(image::formats::Webp.identifier())];
}

+ (FSPEncodedImageFormat *)heif
{
  return [self imageFormatWithIdentifier:FSPStringFromInternalFixedString(image::formats::Heif.identifier())];
}

#pragma mark - Internal

- (image::EncodedFormat)makeInternalFormat
{
  return image::EncodedFormat{FSPInternalFixedStringFromString(self.identifier)};
}

@end

FSPImageFormat *FSPImageFormatFromInternalImageFormat(const image::Format &imageFormat)
{
  if (imageFormat.encoded()) {
    return [FSPEncodedImageFormat imageFormatWithIdentifier:FSPStringFromInternalFixedString(imageFormat.identifier())];
  } else {
    return [FSPImageFormat imageFormatWithIdentifier:FSPStringFromInternalFixedString(imageFormat.identifier())];
  }
}
