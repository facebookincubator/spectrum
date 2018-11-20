// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPEncodeRequirement_Private.h>

using namespace facebook::spectrum;

@interface FSPEncodeRequirementTests : XCTestCase
@end

@implementation FSPEncodeRequirementTests

- (void)testInternalTypeConversion
{
  const auto object = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                   mode:FSPEncodeRequirementModeLossy
                                                                quality:42];

  const auto expectedInternalObject = requirements::Encode {
    .format = image::formats::Jpeg,
    .quality = 42,
    .mode = requirements::Encode::Mode::Lossy,
  };

  XCTAssertTrue([object makeInternalEncodeRequirement] == expectedInternalObject);
}

#pragma mark - Copy

- (void)testCopyIsEqual
{
  const auto object = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                   mode:FSPEncodeRequirementModeLossy
                                                                quality:42];

  XCTAssertEqualObjects(object, [object copy]);
}

#pragma mark - Equality

- (void)testIsEqual
{
  const auto object = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                   mode:FSPEncodeRequirementModeLossy
                                                                quality:42];

  const auto object2 = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                    mode:FSPEncodeRequirementModeLossy
                                                                 quality:42];

  XCTAssertEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentFormat
{
  const auto object = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                   mode:FSPEncodeRequirementModeLossy
                                                                quality:42];
  const auto object2 = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.png
                                                                   mode:FSPEncodeRequirementModeLossy
                                                                quality:42];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentQuality
{
  const auto object = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                   mode:FSPEncodeRequirementModeLossy
                                                                quality:42];
  const auto object2 = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                    mode:FSPEncodeRequirementModeLossy
                                                                 quality:43];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentMode
{
  const auto object = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                   mode:FSPEncodeRequirementModeLossy
                                                                quality:42];
  const auto object2 = [FSPEncodeRequirement encodeRequirementWithFormat:FSPEncodedImageFormat.jpeg
                                                                    mode:FSPEncodeRequirementModeLossless
                                                                 quality:42];

  XCTAssertNotEqualObjects(object, object2);
}

@end
