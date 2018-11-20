// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPOptions_Private.h>

#import <SpectrumKit/FSPAbsoluteToOriginCropRequirement.h>

#include <spectrum/requirements/CropAbsoluteToOrigin.h>

using namespace facebook::spectrum;

@interface FSPEncodeOptionsTests : XCTestCase
@end

@implementation FSPEncodeOptionsTests

- (void)testDefaultValues
{
  const auto encodeRequirement = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg];
  const auto object = [FSPEncodeOptions encodeOptionsWithEncodeRequirement:encodeRequirement];

  XCTAssertNil(object.outputImagePixelSpecification);
  XCTAssertNil(object.metadata);
  XCTAssertNil(object.configuration);
  XCTAssertEqualObjects(object.transformations, [FSPTransformations new]);
}

- (void)testInternalTypeConversion
{
  const auto object = FSPMakeDefaultEncodeOptions();
  const auto internalObject = [object makeInternalEncodeOptions];

  const auto expectedTransformations = Transformations{
    .rotateRequirement = requirements::Rotate{.degrees = 90},
    .cropRequirement = requirements::CropAbsoluteToOrigin({.top = 0, .left = 0, .bottom = 1, .right = 1}, true),
    .resizeRequirement = requirements::Resize{requirements::Resize::Mode::Exact, image::Size{2, 2}},
  };

  const auto expectedEncodeRequirement = requirements::Encode{
    .quality = 42, .format = image::formats::Jpeg, .mode = requirements::Encode::Mode::Lossy};

  XCTAssertTrue(internalObject.transformations == expectedTransformations);
  XCTAssertEqual(internalObject.encodeRequirement, expectedEncodeRequirement);
  XCTAssertEqual(internalObject.outputPixelSpecificationRequirement, image::pixel::specifications::RGB);
}

#pragma mark - Copy

- (void)testCopyIsEqual
{
  const auto object = FSPMakeDefaultEncodeOptions();

  XCTAssertEqualObjects(object, [object copy]);
}

#pragma mark - Equality

- (void)testIsEqual
{
  const auto object = FSPMakeDefaultEncodeOptions();
  const auto object2 = FSPMakeDefaultEncodeOptions();

  XCTAssertEqualObjects(object, object2);
}

#pragma mark - Helpers

static FSPEncodeOptions *FSPMakeDefaultEncodeOptions()
{
  const auto encodeRequirement = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                              mode:FSPEncodeRequirementModeLossy
                                                                           quality:42];

  return [FSPEncodeOptions encodeOptionsWithEncodeRequirement:encodeRequirement
                                              transformations:FSPMakeDefaultTransformations()
                                                     metadata:[FSPImageMetadata new]
                                                configuration:nil
                          outputPixelSpecificationRequirement:FSPImagePixelSpecification.rgb];
}

static FSPTransformations *FSPMakeDefaultTransformations()
{
  const auto transformations = [FSPTransformations new];

  transformations.rotateRequirement = [FSPRotateRequirement rotateRequirementWithDegrees:90];
  transformations.resizeRequirement = [FSPResizeRequirement resizeRequirementExactWithTargetSize:CGSizeMake(2.0f, 2.0f)];
  transformations.cropRequirement = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:{.right = 1, .bottom = 1}];
  return transformations;
}

@end
