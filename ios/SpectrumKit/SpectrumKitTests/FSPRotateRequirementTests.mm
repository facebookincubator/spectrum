// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPRotateRequirement_Private.h>

using namespace facebook::spectrum;

@interface FSPRotateRequirementTests : XCTestCase
@end

@implementation FSPRotateRequirementTests

- (void)testDefaultValues
{
  const auto object = [FSPRotateRequirement rotateRequirementWithDegrees:90];

  XCTAssertEqual(object.degrees, 90);
  XCTAssertFalse(object.shouldFlipHorizontally);
  XCTAssertFalse(object.shouldFlipVertically);
  XCTAssertFalse(object.shouldForceUpOrientation);
}

- (void)testConversionAllTrue
{
  const auto object = [FSPRotateRequirement rotateRequirementWithDegrees:180 shouldFlipHorizontally:YES shouldFlipVertically:YES shouldForceUpOrientation:YES];
  const auto expectedObject = requirements::Rotate{ .degrees = 180, .flipHorizontally = true, .flipVertically = true, .forceUpOrientation = true };

  XCTAssertEqual([object makeInternalRotateRequirement], expectedObject);
}

- (void)testConversionAllFalse
{
  const auto object = [FSPRotateRequirement rotateRequirementWithDegrees:90];
  const auto expectedObject = requirements::Rotate{ .degrees = 90};

  XCTAssertEqual([object makeInternalRotateRequirement], expectedObject);
}

#pragma mark - Equality

- (void)testIsEqual
{
  const auto object = [FSPRotateRequirement rotateRequirementWithDegrees:90];
  const auto object2 = [FSPRotateRequirement rotateRequirementWithDegrees:90];

  XCTAssertEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentDegrees
{
  const auto object = [FSPRotateRequirement rotateRequirementWithDegrees:90 shouldFlipHorizontally:NO shouldFlipVertically:NO shouldForceUpOrientation:NO];
  const auto object2 = [FSPRotateRequirement rotateRequirementWithDegrees:180 shouldFlipHorizontally:NO shouldFlipVertically:NO shouldForceUpOrientation:NO];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentShouldFlipHorizontally
{
  const auto object = [FSPRotateRequirement rotateRequirementWithDegrees:90 shouldFlipHorizontally:NO shouldFlipVertically:NO shouldForceUpOrientation:NO];
  const auto object2 = [FSPRotateRequirement rotateRequirementWithDegrees:90 shouldFlipHorizontally:YES shouldFlipVertically:NO shouldForceUpOrientation:NO];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentShouldFlipVertically
{
  const auto object = [FSPRotateRequirement rotateRequirementWithDegrees:90 shouldFlipHorizontally:NO shouldFlipVertically:NO shouldForceUpOrientation:NO];
  const auto object2 = [FSPRotateRequirement rotateRequirementWithDegrees:90 shouldFlipHorizontally:NO shouldFlipVertically:YES shouldForceUpOrientation:NO];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentShouldForceUpOrientation
{
  const auto object = [FSPRotateRequirement rotateRequirementWithDegrees:90 shouldFlipHorizontally:NO shouldFlipVertically:NO shouldForceUpOrientation:NO];
  const auto object2 = [FSPRotateRequirement rotateRequirementWithDegrees:90 shouldFlipHorizontally:NO shouldFlipVertically:NO shouldForceUpOrientation:YES];

  XCTAssertNotEqualObjects(object, object2);
}

@end
