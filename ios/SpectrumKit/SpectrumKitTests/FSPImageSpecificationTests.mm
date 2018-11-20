// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPImageSpecification.h>
#import <SpectrumKit/FSPImageSpecification_Private.h>

using namespace facebook::spectrum;

@interface FSPImageSpecificationTests : XCTestCase
@end

@implementation FSPImageSpecificationTests

static const CGSize kDefaultSize = CGSizeMake(42.0f, 40.0f);

- (void)testInternalTypeConversion
{
  const auto object = makeDefault();
  const auto expectedInternalObject = image::Specification{
    .size = image::Size{42, 40},
    .format = image::formats::Jpeg,
    .pixelSpecification = image::pixel::specifications::RGB,
    .orientation = image::Orientation::Right,
    .chromaSamplingMode = image::ChromaSamplingMode::S444,
  };

  XCTAssertTrue([object makeInternalImageSpecification] == expectedInternalObject);
}

#pragma mark - Equality

- (void)testIsEqual
{
  const auto object = makeDefault();
  const auto object2 = makeDefault();

  XCTAssertEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentSize
{
  const auto object = makeDefault();
  const auto object2 = [FSPImageSpecification imageSpecificationWithSize:CGSizeMake(1, 1)
                                                                  format:FSPEncodedImageFormat.jpeg
                                                      pixelSpecification:FSPImagePixelSpecification.rgb
                                                             orientation:FSPImageOrientationRight
                                                      chromaSamplingMode:FSPImageChromaSamplingMode444
                                                                metadata:nil];
  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentFormat
{
  const auto object = makeDefault();
  const auto object2 = [FSPImageSpecification imageSpecificationWithSize:kDefaultSize
                                                                  format:FSPEncodedImageFormat.png
                                                      pixelSpecification:FSPImagePixelSpecification.rgb
                                                             orientation:FSPImageOrientationRight
                                                      chromaSamplingMode:FSPImageChromaSamplingMode444
                                                                metadata:nil];
  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentPixelSpecification
{
  const auto object = makeDefault();
  const auto object2 = [FSPImageSpecification imageSpecificationWithSize:kDefaultSize
                                                                  format:FSPEncodedImageFormat.jpeg
                                                      pixelSpecification:FSPImagePixelSpecification.rgba
                                                             orientation:FSPImageOrientationRight
                                                      chromaSamplingMode:FSPImageChromaSamplingMode444
                                                                metadata:nil];
  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentOrientation
{
  const auto object = makeDefault();
  const auto object2 = [FSPImageSpecification imageSpecificationWithSize:kDefaultSize
                                                                  format:FSPEncodedImageFormat.jpeg
                                                      pixelSpecification:FSPImagePixelSpecification.rgb
                                                             orientation:FSPImageOrientationBottom
                                                      chromaSamplingMode:FSPImageChromaSamplingMode444
                                                                metadata:nil];
  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentMetadata
{
  const auto object = makeDefault();
  const auto metadata = [FSPImageMetadata imageMetadataWithDictionary:@{
    (NSString *)kCGImagePropertyTIFFDictionary: @{
        (NSString *)kCGImagePropertyTIFFOrientation: @2,
        },
  }];

  const auto object2 = [FSPImageSpecification imageSpecificationWithSize:kDefaultSize
                                                                  format:FSPEncodedImageFormat.jpeg
                                                      pixelSpecification:FSPImagePixelSpecification.rgba
                                                             orientation:FSPImageOrientationRight
                                                      chromaSamplingMode:FSPImageChromaSamplingMode444
                                                                metadata:metadata];
  XCTAssertNotEqualObjects(object, object2);
}

static FSPImageSpecification *makeDefault()
{
  return [FSPImageSpecification imageSpecificationWithSize:kDefaultSize
                                                    format:FSPEncodedImageFormat.jpeg
                                        pixelSpecification:FSPImagePixelSpecification.rgb
                                               orientation:FSPImageOrientationRight
                                        chromaSamplingMode:FSPImageChromaSamplingMode444
                                                  metadata:nil];
}
@end
