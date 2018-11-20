// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPImageFormat.h>
#import <SpectrumKit/FSPImageFormat_Private.h>

using namespace facebook::spectrum;

@interface FSPImageFormatTests : XCTestCase
@end

@implementation FSPImageFormatTests

#pragma mark - Image Format

- (void)testImageFormatPNG
{
  [self testEqualityOfImageFormat:FSPEncodedImageFormat.png internalImageFormat:image::formats::Png];
}

- (void)testImageFormatJPEG
{
  [self testEqualityOfImageFormat:FSPEncodedImageFormat.jpeg internalImageFormat:image::formats::Jpeg];
}

- (void)testImageFormatBitmapRGBA
{
  [self testEqualityOfImageFormat:FSPImageFormat.bitmap internalImageFormat:image::formats::Bitmap];
}

- (void)testImageFormatGIF
{
  [self testEqualityOfImageFormat:FSPEncodedImageFormat.gif internalImageFormat:image::formats::Gif];
}

- (void)testImageFormatWEBP
{
  [self testEqualityOfImageFormat:FSPEncodedImageFormat.webp internalImageFormat:image::formats::Webp];
}

#pragma mark - Private

- (void)testEqualityOfImageFormat:(FSPImageFormat *)imageFormat internalImageFormat:(const image::Format &)internalImageFormat
{
  XCTAssertEqualObjects(FSPImageFormatFromInternalImageFormat(internalImageFormat), imageFormat);
  XCTAssertEqual([imageFormat makeInternalFormat], internalImageFormat);
}

@end
