// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPImageSpecification.h"
#import "FSPImageSpecification_Private.h"

#import "FSPLog.h"

#import "FSPGeometry_Private.h"
#import "FSPImageFormat_Private.h"
#import "FSPImageOrientation_Private.h"
#import "FSPImagePixelSpecification_Private.h"
#import "FSPPrivate.h"
#import "FSPImageMetadata_Private.h"

using namespace facebook::spectrum;

@implementation FSPImageSpecification

#pragma mark - Convenience Initializers

+ (instancetype)imageSpecificationWithSize:(CGSize)size
                                    format:(FSPImageFormat *)format
                        pixelSpecification:(FSPImagePixelSpecification *)pixelSpecification
                               orientation:(FSPImageOrientation)orientation
                        chromaSamplingMode:(FSPImageChromaSamplingMode)chromaSamplingMode
                                  metadata:(nullable FSPImageMetadata *)metadata
{
  return [[self alloc] initWithSize:size
                             format:format
                 pixelSpecification:pixelSpecification
                        orientation:orientation
                 chromaSamplingMode:chromaSamplingMode
                           metadata:metadata];
}

#pragma mark - Initialisers

- (instancetype)initWithSize:(CGSize)size
                      format:(FSPImageFormat *)format
          pixelSpecification:(FSPImagePixelSpecification *)pixelSpecification
                 orientation:(FSPImageOrientation)orientation
          chromaSamplingMode:(FSPImageChromaSamplingMode)chromaSamplingMode
                    metadata:(nullable FSPImageMetadata *)metadata
{
  FSPReportMustFixIfNil(format, nil);
  FSPReportMustFixIfNil(pixelSpecification, nil);

  if (self = [super init]) {
    _size = size;
    _format = format;
    _pixelSpecification = pixelSpecification;
    _orientation = orientation;
    _chromaSamplingMode = chromaSamplingMode;
    _metadata = metadata ?: [FSPImageMetadata new];
  }

  return self;
}

#pragma mark - Equality

- (BOOL)isEqualToImageSpecification:(FSPImageSpecification *)object
{
  if (object == nil) {
    return NO;
  }

  return (CGSizeEqualToSize(_size, object.size) &&
          FSPObjectEqualObject(_format, object.format) &&
          FSPObjectEqualObject(_pixelSpecification, object.pixelSpecification) &&
          FSPObjectEqualObject(_metadata, object.metadata) &&
          _chromaSamplingMode == object.chromaSamplingMode &&
          _orientation == object.orientation);
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPImageSpecification class]]) {
    return [self isEqualToImageSpecification:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return @(_size.width).hash ^ @(_size.height).hash ^ _format.hash ^ _pixelSpecification.hash ^ _orientation ^ _chromaSamplingMode ^ _metadata.hash;
}

#pragma mark - Internal

+ (instancetype)imageSpecificationFromInternalImageSpecification:(const image::Specification &)imageSpecification
{
  return [[self alloc] initWithSize:FSPSizeFromInternalImageSize(imageSpecification.size)
                             format:FSPImageFormatFromInternalImageFormat(imageSpecification.format)
                 pixelSpecification:[FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:imageSpecification.pixelSpecification]
                        orientation:FSPImageOrientationFromInternalImageOrientation(imageSpecification.orientation)
                 chromaSamplingMode:FSPImageChromaSamplingModeFromInternalChromaSamplingMode(imageSpecification.chromaSamplingMode)
                          // TODO T33881521: Remove when metadata Cpp to Objc metadata conversion done
                           metadata:[FSPImageMetadata new]];
}

- (image::Specification)makeInternalImageSpecification
{
  return image::Specification{
    .size = FSPInternalImageSizeFromCGSize(_size),
    .format = [_format makeInternalFormat],
    .pixelSpecification = [_pixelSpecification makeInternalSpecification],
    .orientation = FSPInternalImageOrientationFromImageOrientation(_orientation),
    .chromaSamplingMode = FSPInternalChromaSamplingModeFromChromaSamplingMode(_chromaSamplingMode),
    .metadata = [_metadata makeInternalMetadata],
  };
}

+ (instancetype)imageSpecificationFromImage:(UIImage *)image
{
  const auto imageSize = CGSizeMake(CGImageGetWidth(image.CGImage), CGImageGetHeight(image.CGImage));
  return [[self alloc] initWithSize:imageSize
                             format:FSPImageFormat.bitmap
                 pixelSpecification:[FSPImagePixelSpecification imagePixelSpecificationFromImage:image]
                        orientation:FSPImageOrientationFromImage(image)
                 chromaSamplingMode:FSPImageChromaSamplingModeNone
                           metadata:[FSPImageMetadata imageMetadataFromImage:image]];
}

static folly::Optional<image::ChromaSamplingMode> FSPInternalChromaSamplingModeFromChromaSamplingMode(FSPImageChromaSamplingMode chromaSamplingMode)
{
  switch (chromaSamplingMode) {
    case FSPImageChromaSamplingModeNone:
      return folly::none;
    case FSPImageChromaSamplingMode444:
      return image::ChromaSamplingMode::S444;
    case FSPImageChromaSamplingMode420:
      return image::ChromaSamplingMode::S420;
    case FSPImageChromaSamplingMode422:
      return image::ChromaSamplingMode::S422;
    case FSPImageChromaSamplingMode411:
      return image::ChromaSamplingMode::S411;
    case FSPImageChromaSamplingMode440:
      return image::ChromaSamplingMode::S440;
  }
}

static FSPImageChromaSamplingMode FSPImageChromaSamplingModeFromInternalChromaSamplingMode(const folly::Optional<image::ChromaSamplingMode>& chromaSamplingMode)
{
  if (chromaSamplingMode.hasValue()) {
    switch (*chromaSamplingMode) {
      case image::ChromaSamplingMode::S444:
        return FSPImageChromaSamplingMode444;
      case image::ChromaSamplingMode::S420:
        return FSPImageChromaSamplingMode420;
      case image::ChromaSamplingMode::S422:
        return FSPImageChromaSamplingMode422;
      case image::ChromaSamplingMode::S411:
        return FSPImageChromaSamplingMode411;
      case image::ChromaSamplingMode::S440:
        return FSPImageChromaSamplingMode440;
    }
  } else {
    return FSPImageChromaSamplingModeNone;
  }
}

@end
