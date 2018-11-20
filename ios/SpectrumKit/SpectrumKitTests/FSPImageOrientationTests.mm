// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPImageOrientation_Private.h>

using namespace facebook::spectrum;

@interface FSPImageOrientationTests : XCTestCase
@end

@implementation FSPImageOrientationTests

- (void)testOrientationUp
{
  [self testEqualityOfImageOrientation:FSPImageOrientationUp internalImageOrientation:image::Orientation::Up];
}

- (void)testOrientationRight
{
  [self testEqualityOfImageOrientation:FSPImageOrientationRight internalImageOrientation:image::Orientation::Right];
}

- (void)testOrientationBottom
{
  [self testEqualityOfImageOrientation:FSPImageOrientationBottom internalImageOrientation:image::Orientation::Bottom];
}

- (void)testOrientationLeft
{
  [self testEqualityOfImageOrientation:FSPImageOrientationLeft internalImageOrientation:image::Orientation::Left];
}

- (void)testOrientationUpMirrored
{
  [self testEqualityOfImageOrientation:FSPImageOrientationUpMirrored internalImageOrientation:image::Orientation::UpMirrored];
}

- (void)testOrientationRightMirrored
{
  [self testEqualityOfImageOrientation:FSPImageOrientationRightMirrored internalImageOrientation:image::Orientation::RightMirrored];
}

- (void)testOrientationBottomMirrored
{
  [self testEqualityOfImageOrientation:FSPImageOrientationBottomMirrored internalImageOrientation:image::Orientation::BottomMirrored];
}

- (void)testOrientationLeftMirrored
{
  [self testEqualityOfImageOrientation:FSPImageOrientationLeftMirrored internalImageOrientation:image::Orientation::LeftMirrored];
}

#pragma mark - Private

- (void)testEqualityOfImageOrientation:(FSPImageOrientation)imageOrientation internalImageOrientation:(const image::Orientation)internalImageOrientation
{
  XCTAssertEqual(FSPImageOrientationFromInternalImageOrientation(internalImageOrientation), imageOrientation);
  XCTAssertEqual(FSPInternalImageOrientationFromImageOrientation(imageOrientation), internalImageOrientation);
}

@end
