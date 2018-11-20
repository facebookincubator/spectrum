// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPImageFormat.h>
#include <spectrum/image/Format.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPImageFormat (Private)

- (facebook::spectrum::image::Format)makeInternalFormat;

@end

@interface FSPEncodedImageFormat (Private)

- (facebook::spectrum::image::EncodedFormat)makeInternalFormat;

@end

FSPImageFormat *FSPImageFormatFromInternalImageFormat(const facebook::spectrum::image::Format &imageFormat);
FSPEncodedImageFormat *FSPEncodedImageFormatFromInternalImageFormat(const facebook::spectrum::image::EncodedFormat &imageFormat);

NS_ASSUME_NONNULL_END
