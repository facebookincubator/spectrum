// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPImageFormat.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * A color model defines the image's components and how they are interpreted to
 * recreate the intended color.
 */
NS_SWIFT_NAME(ImagePixelSpecificationColorModel)
@interface FSPImagePixelSpecificationColorModel : NSObject

+ (instancetype)new NS_UNAVAILABLE;

/** Color model consisting of a gray color channel */
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecificationColorModel *gray;

/**
 * Color model consisting of a red, green and blue color channel which work additively.
 */
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecificationColorModel *rgb;

@property (nonatomic, copy, readonly, class) FSPImagePixelSpecificationColorModel *yCbCr;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecificationColorModel *yCbCrK;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecificationColorModel *unknown;

/**
 * Human readable identifier of the color model (e.g. `RGB`).
 */
@property (nonatomic, copy, readonly) NSString *identifier;

/**
 * Number of independent color components in the color model (e.g. 3 for for `RGB`).
 */
@property (nonatomic, assign, readonly) NSUInteger numberOfComponents;

/**
 * True if the color model supports an alpha channel. For example, `RGB`
 * supports an alpha channel whereas `CMYK` does not.
 */
@property (nonatomic, assign, readonly) BOOL supportsExtraAlphaComponent;

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithIdentifier:(NSString *)identifier
                numberOfComponents:(NSUInteger)numberOfComponents
       supportsExtraAlphaComponent:(BOOL)supportsExtraAlphaComponent NS_DESIGNATED_INITIALIZER;

- (BOOL)isEqualToImagePixelSpecificationColorModel:(FSPImagePixelSpecificationColorModel *)object;

@end

/**
 * The alpha info specifies the behaviour of the optional alpha channel.
 */
typedef NS_ENUM(NSUInteger, FSPImagePixelSpecificationAlphaInfo) {
  FSPImagePixelSpecificationAlphaInfoNone,
  FSPImagePixelSpecificationAlphaInfoFirst,
  FSPImagePixelSpecificationAlphaInfoLast,
  FSPImagePixelSpecificationAlphaInfoSkipFirst,
  FSPImagePixelSpecificationAlphaInfoSkipLast,
  FSPImagePixelSpecificationAlphaInfoPremultipliedFirst,
  FSPImagePixelSpecificationAlphaInfoPremultipliedLast,
} NS_SWIFT_NAME(ImagePixelSpecificationAlphaInfo);

/** Order of the components in memory representation */
typedef NS_ENUM(NSUInteger, FSPImagePixelSpecificationComponentsOrder) {
  FSPImagePixelSpecificationComponentsOrderNatural,
  FSPImagePixelSpecificationComponentsOrderReversed,
} NS_SWIFT_NAME(ImagePixelSpecificationComponentsOrder);

/**
 * The specification explains how pixels and the color information is stored as
 * a continuous stream of bytes.
 */
NS_SWIFT_NAME(ImagePixelSpecification)
@interface FSPImagePixelSpecification : NSObject

+ (instancetype)new NS_UNAVAILABLE;

+ (instancetype)imagePixelSpecificationWithColorModel:(FSPImagePixelSpecificationColorModel *)colorModel
                                        bytesPerPixel:(NSUInteger)bytesPerPixel
                                            alphaInfo:(FSPImagePixelSpecificationAlphaInfo)alphaInfo
                                      componentsOrder:(FSPImagePixelSpecificationComponentsOrder)componentsOrder;

@property (nonatomic, copy, readonly, class) FSPImagePixelSpecification *gray;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecification *grayA;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecification *aGray;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecification *rgb;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecification *rgba;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecification *argb;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecification *bgr;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecification *bgra;
@property (nonatomic, copy, readonly, class) FSPImagePixelSpecification *abgr;

/**
 * Number of color components and how to interpret them.
 */
@property (nonatomic, strong, readonly) FSPImagePixelSpecificationColorModel *colorModel;

/**
 * The number of bytes per pixel (covering all its components).
 */
@property (nonatomic, assign, readonly) NSUInteger bytesPerPixel;

/**
 * Information about the optional alpha channel.
 */
@property (nonatomic, assign, readonly) FSPImagePixelSpecificationAlphaInfo alphaInfo;

/**
 * If true, the components are ordered reverse to the default of the color
 * model. E.g. BGR instead of RGB.
 */
@property (nonatomic, assign, readonly) FSPImagePixelSpecificationComponentsOrder componentsOrder;

- (instancetype)init NS_UNAVAILABLE;

- (BOOL)isEqualToImagePixelSpecification:(FSPImagePixelSpecification *)object;

@end

NS_ASSUME_NONNULL_END
