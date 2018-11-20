// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPImagePixelSpecification.h"
#import "FSPImagePixelSpecification_Private.h"

#import "FSPLog.h"
#include <spectrum/core/utils/Numeric.h>

#import "FSPPrivate.h"

using namespace facebook::spectrum;

@implementation FSPImagePixelSpecification

+ (instancetype)imagePixelSpecificationWithColorModel:(FSPImagePixelSpecificationColorModel *)colorModel
                                        bytesPerPixel:(NSUInteger)bytesPerPixel
                                            alphaInfo:(FSPImagePixelSpecificationAlphaInfo)alphaInfo
                                      componentsOrder:(FSPImagePixelSpecificationComponentsOrder)componentsOrder
{
  return [[self alloc] initWithColorModel:colorModel
                            bytesPerPixel:bytesPerPixel
                                alphaInfo:alphaInfo
                          componentsOrder:componentsOrder];
}

+ (instancetype)imagePixelSpecificationFromInternalPixelSpecification:(const image::pixel::Specification &)internalPixelSpecification
{
  return [self imagePixelSpecificationWithColorModel:[FSPImagePixelSpecificationColorModel colorModelWithInternalColorModel:internalPixelSpecification.colorModel]
                                  bytesPerPixel:SPECTRUM_CONVERT_OR_THROW(internalPixelSpecification.bytesPerPixel, NSUInteger)
                                      alphaInfo:FSPImagePixelSpecificationAlphaInfoFromInternalAlphaInfo(internalPixelSpecification.alphaInfo)
                                componentsOrder:FSPImagePixelSpecificationComponentsOrderFromInternalComponentsOrder(internalPixelSpecification.componentsOrder)];
}

- (instancetype)initWithColorModel:(FSPImagePixelSpecificationColorModel *)colorModel
                     bytesPerPixel:(NSUInteger)bytesPerPixel
                         alphaInfo:(FSPImagePixelSpecificationAlphaInfo)alphaInfo
                   componentsOrder:(FSPImagePixelSpecificationComponentsOrder)componentsOrder
{
  FSPReportMustFixIfNil(colorModel, nil);

  if (self = [super init]) {
    _colorModel = colorModel;
    _bytesPerPixel = bytesPerPixel;
    _alphaInfo = alphaInfo;
    _componentsOrder = componentsOrder;
  }

  return self;
}

#pragma mark - Equality

- (BOOL)isEqualToImagePixelSpecification:(FSPImagePixelSpecification *)object
{
  if (object == nil) {
    return NO;
  }

  return (FSPObjectEqualObject(_colorModel, object.colorModel) &&
          _bytesPerPixel == object.bytesPerPixel &&
          _alphaInfo == object.alphaInfo &&
          _componentsOrder == object.componentsOrder);
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPImagePixelSpecification class]]) {
    return [self isEqualToImagePixelSpecification:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return _colorModel.hash ^ _bytesPerPixel ^ _alphaInfo ^ _componentsOrder;
}

#pragma mark - Class Properties

+ (FSPImagePixelSpecification *)gray
{
  return [FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:image::pixel::specifications::Gray];
}

+ (FSPImagePixelSpecification *)grayA
{
  return [FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:image::pixel::specifications::GrayA];
}

+ (FSPImagePixelSpecification *)aGray
{
  return [FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:image::pixel::specifications::AGray];
}

+ (FSPImagePixelSpecification *)rgb
{
  return [FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:image::pixel::specifications::RGB];
}

+ (FSPImagePixelSpecification *)rgba
{
  return [FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:image::pixel::specifications::RGBA];
}

+ (FSPImagePixelSpecification *)argb
{
  return [FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:image::pixel::specifications::ARGB];
}

+ (FSPImagePixelSpecification *)bgr
{
  return [FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:image::pixel::specifications::BGR];
}

+ (FSPImagePixelSpecification *)bgra
{
  return [FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:image::pixel::specifications::BGRA];
}

+ (FSPImagePixelSpecification *)abgr
{
  return [FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:image::pixel::specifications::ABGR];
}

#pragma mark - Internal

- (image::pixel::Specification)makeInternalSpecification
{
  return image::pixel::Specification {
    [_colorModel makeInternalColorModel],
    SPECTRUM_CONVERT_OR_THROW(_bytesPerPixel, std::uint8_t),
    FSPInternalImagePixelSpecificationAlphaInfoFromAlphaInfo(_alphaInfo),
    FSPInternalImagePixelSpecificationComponentsOrderFromComponentsOrder(_componentsOrder),
  };
}

#pragma mark - Internal

image::pixel::AlphaInfo FSPInternalImagePixelSpecificationAlphaInfoFromAlphaInfo(const FSPImagePixelSpecificationAlphaInfo alphaInfo)
{
  switch (alphaInfo) {
    case FSPImagePixelSpecificationAlphaInfoNone:
      return image::pixel::AlphaInfo::None;
    case FSPImagePixelSpecificationAlphaInfoFirst:
      return image::pixel::AlphaInfo::First;
    case FSPImagePixelSpecificationAlphaInfoLast:
      return image::pixel::AlphaInfo::Last;
    case FSPImagePixelSpecificationAlphaInfoSkipFirst:
      return image::pixel::AlphaInfo::SkipFirst;
    case FSPImagePixelSpecificationAlphaInfoSkipLast:
      return image::pixel::AlphaInfo::SkipLast;
    case FSPImagePixelSpecificationAlphaInfoPremultipliedFirst:
      return image::pixel::AlphaInfo::PremultipliedFirst;
    case FSPImagePixelSpecificationAlphaInfoPremultipliedLast:
      return image::pixel::AlphaInfo::PremultipliedLast;
  }
}

image::pixel::ComponentsOrder FSPInternalImagePixelSpecificationComponentsOrderFromComponentsOrder(const FSPImagePixelSpecificationComponentsOrder componentsOrder)
{
  switch (componentsOrder) {
    case FSPImagePixelSpecificationComponentsOrderNatural:
      return image::pixel::ComponentsOrder::Natural;
    case FSPImagePixelSpecificationComponentsOrderReversed:
      return image::pixel::ComponentsOrder::Reversed;
  }
}

FSPImagePixelSpecificationAlphaInfo FSPImagePixelSpecificationAlphaInfoFromInternalAlphaInfo(const image::pixel::AlphaInfo alphaInfo)
{
  switch (alphaInfo) {
    case image::pixel::AlphaInfo::None:
      return FSPImagePixelSpecificationAlphaInfoNone;
    case image::pixel::AlphaInfo::First:
      return FSPImagePixelSpecificationAlphaInfoFirst;
    case image::pixel::AlphaInfo::Last:
      return FSPImagePixelSpecificationAlphaInfoLast;
    case image::pixel::AlphaInfo::SkipFirst:
      return FSPImagePixelSpecificationAlphaInfoSkipFirst;
    case image::pixel::AlphaInfo::SkipLast:
      return FSPImagePixelSpecificationAlphaInfoSkipLast;
    case image::pixel::AlphaInfo::PremultipliedFirst:
      return FSPImagePixelSpecificationAlphaInfoPremultipliedFirst;
    case image::pixel::AlphaInfo::PremultipliedLast:
      return FSPImagePixelSpecificationAlphaInfoPremultipliedLast;
  }
}

FSPImagePixelSpecificationComponentsOrder FSPImagePixelSpecificationComponentsOrderFromInternalComponentsOrder(const image::pixel::ComponentsOrder componentsOrder)
{
  switch (componentsOrder) {
    case image::pixel::ComponentsOrder::Natural:
      return FSPImagePixelSpecificationComponentsOrderNatural;
    case image::pixel::ComponentsOrder::Reversed:
      return FSPImagePixelSpecificationComponentsOrderReversed;
  }
}

+ (instancetype)imagePixelSpecificationFromImage:(UIImage *)image
{
  return [[self alloc] initWithColorModel:[FSPImagePixelSpecificationColorModel imagePixelSpecificationColorModelFromImage:image]
                            bytesPerPixel:CGImageGetBitsPerPixel(image.CGImage) / 8
                                alphaInfo:FSPImagePixelSpecificationAlphaInfoFromImage(image)
                          componentsOrder:FSPImagePixelSpecificationComponentsOrderFromImage(image)];
}

@end

@implementation FSPImagePixelSpecificationColorModel

+ (instancetype)colorModelWithInternalColorModel:(const facebook::spectrum::image::pixel::ColorModel &)colorModel
{
  return [[self alloc] initWithIdentifier:FSPStringFromInternalFixedString(colorModel.identifier)
                       numberOfComponents:SPECTRUM_CONVERT_OR_THROW(colorModel.numberOfColorComponents, NSUInteger)
              supportsExtraAlphaComponent:SPECTRUM_CONVERT_OR_THROW(colorModel.supportsExtraAlphaChannel, BOOL)];
}

+ (instancetype)imagePixelSpecificationColorModelFromImage:(UIImage *)image
{
  const auto colorSpace = CGImageGetColorSpace(image.CGImage);
  const auto colorModel = CGColorSpaceGetModel(colorSpace);

  switch (colorModel) {
    case kCGColorSpaceModelUnknown:
      return FSPImagePixelSpecificationColorModel.unknown;
    case kCGColorSpaceModelMonochrome:
      return FSPImagePixelSpecificationColorModel.gray;
    case kCGColorSpaceModelRGB:
      return FSPImagePixelSpecificationColorModel.rgb;
    case kCGColorSpaceModelCMYK:
      return FSPImagePixelSpecificationColorModel.yCbCr;
    case kCGColorSpaceModelLab:
    case kCGColorSpaceModelDeviceN:
    case kCGColorSpaceModelIndexed:
    case kCGColorSpaceModelPattern:
    default: // case kCGColorSpaceModelXYZ:
      SPECTRUM_ERROR_FORMAT("unsupported_image_pixel_spec_color_model", "%d", colorModel);
  }
}

- (instancetype)initWithIdentifier:(NSString *)identifier
                numberOfComponents:(NSUInteger)numberOfComponents
       supportsExtraAlphaComponent:(BOOL)supportsExtraAlphaComponent
{
  if (self = [super init]) {
    _identifier = [identifier copy];
    _numberOfComponents = numberOfComponents;
    _supportsExtraAlphaComponent = supportsExtraAlphaComponent;
  }

  return self;
}

#pragma mark - Equality

- (BOOL)isEqualToImagePixelSpecificationColorModel:(FSPImagePixelSpecificationColorModel *)object
{
  if (object == nil) {
    return NO;
  }

  return (FSPObjectEqualObject(_identifier, object.identifier) &&
          _numberOfComponents == object.numberOfComponents &&
          _supportsExtraAlphaComponent == object.supportsExtraAlphaComponent);
}

#pragma mark - NSObject

- (BOOL)isEqual:(id)object
{
  if (object == self) {
    return YES;
  }

  if ([object isKindOfClass:[FSPImagePixelSpecificationColorModel class]]) {
    return [self isEqualToImagePixelSpecificationColorModel:object];
  } else {
    return NO;
  }
}

- (NSUInteger)hash
{
  return _identifier.hash ^ _numberOfComponents ^ static_cast<NSUInteger>(_supportsExtraAlphaComponent);
}

#pragma mark - Class Properties

+ (FSPImagePixelSpecificationColorModel *)gray
{
  return [FSPImagePixelSpecificationColorModel colorModelWithInternalColorModel:image::pixel::colormodels::Gray];
}

+ (FSPImagePixelSpecificationColorModel *)rgb
{
  return [FSPImagePixelSpecificationColorModel colorModelWithInternalColorModel:image::pixel::colormodels::RGB];
}

+ (FSPImagePixelSpecificationColorModel *)yCbCr
{
  return [FSPImagePixelSpecificationColorModel colorModelWithInternalColorModel:image::pixel::colormodels::YCbCr];
}

+ (FSPImagePixelSpecificationColorModel *)yCbCrK
{
  return [FSPImagePixelSpecificationColorModel colorModelWithInternalColorModel:image::pixel::colormodels::YCbCrK];
}

+ (FSPImagePixelSpecificationColorModel *)unknown
{
  return [FSPImagePixelSpecificationColorModel colorModelWithInternalColorModel:image::pixel::colormodels::Unknown];
}

#pragma mark - Internal

- (image::pixel::ColorModel)makeInternalColorModel
{
  return image::pixel::ColorModel{
    FSPInternalFixedStringFromString(self.identifier),
    SPECTRUM_CONVERT_OR_THROW(self.numberOfComponents, std::uint8_t),
    SPECTRUM_CONVERT_OR_THROW(self.supportsExtraAlphaComponent, bool),
  };
}

@end

FSPImagePixelSpecificationComponentsOrder FSPImagePixelSpecificationComponentsOrderFromImage(UIImage *image)
{
  const auto bitmapInfo = CGImageGetBitmapInfo(image.CGImage);
  const auto bitmapByteOrder = static_cast<CGImageByteOrderInfo>(bitmapInfo & kCGImageByteOrderMask);

  if ((bitmapByteOrder & kCGImageByteOrder32Big) == kCGBitmapByteOrder32Host ||
      (bitmapByteOrder & kCGImageByteOrder16Big) == kCGBitmapByteOrder16Host ||
      (bitmapByteOrder & kCGImageByteOrder32Little) == kCGBitmapByteOrder32Host ||
      (bitmapByteOrder & kCGImageByteOrder16Little) == kCGBitmapByteOrder16Host) {
    return FSPImagePixelSpecificationComponentsOrderReversed;
  } else {
    return FSPImagePixelSpecificationComponentsOrderNatural;
  }
}

FSPImagePixelSpecificationAlphaInfo FSPImagePixelSpecificationAlphaInfoFromImage(UIImage *image)
{
  switch (CGImageGetAlphaInfo(image.CGImage)) {
    case kCGImageAlphaNone:
      return FSPImagePixelSpecificationAlphaInfoNone;
    case kCGImageAlphaPremultipliedLast:
      return FSPImagePixelSpecificationAlphaInfoPremultipliedLast;
    case kCGImageAlphaPremultipliedFirst:
      return FSPImagePixelSpecificationAlphaInfoPremultipliedFirst;
    case kCGImageAlphaLast:
      return FSPImagePixelSpecificationAlphaInfoLast;
    case kCGImageAlphaFirst:
      return FSPImagePixelSpecificationAlphaInfoFirst;
    case kCGImageAlphaNoneSkipLast:
      return FSPImagePixelSpecificationAlphaInfoSkipLast;
    case kCGImageAlphaNoneSkipFirst:
      return FSPImagePixelSpecificationAlphaInfoSkipFirst;
    case kCGImageAlphaOnly:
      SPECTRUM_ERROR("unsupported_image_pixel_spec_alpha_only");
  }
}
