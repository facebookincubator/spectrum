// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIImage.h>

#import <SpectrumKit/FSPImagePixelSpecification.h>
#include <spectrum/image/Pixel.h>

NS_ASSUME_NONNULL_BEGIN

facebook::spectrum::image::pixel::AlphaInfo FSPInternalImagePixelSpecificationAlphaInfoFromAlphaInfo(FSPImagePixelSpecificationAlphaInfo alphaInfo);
FSPImagePixelSpecificationAlphaInfo FSPImagePixelSpecificationAlphaInfoFromInternalAlphaInfo(const facebook::spectrum::image::pixel::AlphaInfo alphaInfo);
FSPImagePixelSpecificationAlphaInfo FSPImagePixelSpecificationAlphaInfoFromImage(UIImage *image);

facebook::spectrum::image::pixel::ComponentsOrder FSPInternalImagePixelSpecificationComponentsOrderFromComponentsOrder(FSPImagePixelSpecificationComponentsOrder componentsOrder);
FSPImagePixelSpecificationComponentsOrder FSPImagePixelSpecificationComponentsOrderFromInternalComponentsOrder(const facebook::spectrum::image::pixel::ComponentsOrder componentsOrder);
FSPImagePixelSpecificationComponentsOrder FSPImagePixelSpecificationComponentsOrderFromImage(UIImage *image);

@interface FSPImagePixelSpecification (Private)

+ (instancetype)imagePixelSpecificationFromImage:(UIImage *)image;

+ (instancetype)imagePixelSpecificationFromInternalPixelSpecification:(const facebook::spectrum::image::pixel::Specification &)internalPixelSpecification;

- (facebook::spectrum::image::pixel::Specification)makeInternalSpecification;

@end

@interface FSPImagePixelSpecificationColorModel (Private)

+ (instancetype)imagePixelSpecificationColorModelFromImage:(UIImage *)image;

+ (instancetype)colorModelWithInternalColorModel:(const facebook::spectrum::image::pixel::ColorModel &)colorModel;

- (facebook::spectrum::image::pixel::ColorModel)makeInternalColorModel;

@end

NS_ASSUME_NONNULL_END
