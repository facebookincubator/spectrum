// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPImagePixelSpecification.h>
#import <SpectrumKit/FSPImagePixelSpecification_Private.h>

using namespace facebook::spectrum;

@interface FSPImagePixelSpecificationTests : XCTestCase
@end

@implementation FSPImagePixelSpecificationTests

- (void)testPixelSpecificationConversionNone
{
  [self testEqualityOfPixelSpecification:nil internalPixelSpecification:folly::none];
}

- (void)testPixelConversionGray
{
  [self testEqualityOfPixelSpecification:FSPImagePixelSpecification.gray internalPixelSpecification:image::pixel::specifications::Gray];
}

- (void)testPixelSpecificationConversionRgb
{
  [self testEqualityOfPixelSpecification:FSPImagePixelSpecification.rgb internalPixelSpecification:image::pixel::specifications::RGB];
}

- (void)testPixelSpecificationConversionArgb
{
  [self testEqualityOfPixelSpecification:FSPImagePixelSpecification.argb internalPixelSpecification:image::pixel::specifications::ARGB];
}

- (void)testPixelSpecificationConversionRgba
{
  [self testEqualityOfPixelSpecification:FSPImagePixelSpecification.rgba internalPixelSpecification:image::pixel::specifications::RGBA];
}

- (void)testPixelSpecificationConversionBgr
{
  [self testEqualityOfPixelSpecification:FSPImagePixelSpecification.bgr internalPixelSpecification:image::pixel::specifications::BGR];
}

- (void)testPixelSpecificationConversionAbgr
{
  [self testEqualityOfPixelSpecification:FSPImagePixelSpecification.abgr internalPixelSpecification:image::pixel::specifications::ABGR];
}

- (void)testPixelSpecificationConversionBgra
{
  [self testEqualityOfPixelSpecification:FSPImagePixelSpecification.bgra internalPixelSpecification:image::pixel::specifications::BGRA];
}

#pragma mark - Copy

- (void)testCopyIsEqual
{

}

#pragma mark - Equality

- (void)testIsEqual
{
  XCTAssertEqualObjects(FSPImagePixelSpecification.rgb, FSPImagePixelSpecification.rgb);
}

- (void)testIsNotEqualOnDifferentColorModel
{
  const auto object = FSPImagePixelSpecification.rgb;
  const auto object2 = [FSPImagePixelSpecification imagePixelSpecificationWithColorModel:FSPImagePixelSpecificationColorModel.gray
                                                                           bytesPerPixel:3
                                                                               alphaInfo:FSPImagePixelSpecificationAlphaInfoNone
                                                                         componentsOrder:FSPImagePixelSpecificationComponentsOrderNatural];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentBytesPerPixel
{
  const auto object = FSPImagePixelSpecification.rgb;
  const auto object2 = [FSPImagePixelSpecification imagePixelSpecificationWithColorModel:FSPImagePixelSpecificationColorModel.rgb
                                                                           bytesPerPixel:4
                                                                               alphaInfo:FSPImagePixelSpecificationAlphaInfoNone
                                                                         componentsOrder:FSPImagePixelSpecificationComponentsOrderNatural];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentAlphaInfo
{
  const auto object = FSPImagePixelSpecification.rgb;
  const auto object2 = [FSPImagePixelSpecification imagePixelSpecificationWithColorModel:FSPImagePixelSpecificationColorModel.rgb
                                                                           bytesPerPixel:3
                                                                               alphaInfo:FSPImagePixelSpecificationAlphaInfoFirst
                                                                         componentsOrder:FSPImagePixelSpecificationComponentsOrderNatural];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentComponentsOrder
{
  const auto object = FSPImagePixelSpecification.rgb;
  const auto object2 = FSPImagePixelSpecification.bgr;

  XCTAssertNotEqualObjects(object, object2);
}

#pragma mark - Private

- (void)testEqualityOfPixelSpecification:(nullable FSPImagePixelSpecification *const)pixelSpecification
              internalPixelSpecification:(const folly::Optional<image::pixel::Specification> &)internalPixelSpecification
{
  if (pixelSpecification == nil) {
    XCTAssertTrue(internalPixelSpecification == folly::none);
  } else {
    XCTAssertEqual([pixelSpecification makeInternalSpecification], internalPixelSpecification);
  }

  if (internalPixelSpecification.hasValue()) {
    XCTAssertEqualObjects([FSPImagePixelSpecification imagePixelSpecificationFromInternalPixelSpecification:*internalPixelSpecification], pixelSpecification);
  } else {
    XCTAssertNil(pixelSpecification);
  }
}

@end
