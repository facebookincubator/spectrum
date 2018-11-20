// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPGeometry_Private.h>

using namespace facebook::spectrum;

@interface FSPGeometryInternalTests : XCTestCase
@end

@implementation FSPGeometryInternalTests

#pragma mark - Size

- (void)testSizeToImageSizeIsNone
{
  XCTAssertTrue(FSPInternalImageSizeOptionalFromCGSize(CGSizeZero) == folly::none);
}

- (void)testSizeToImageSizeIsEqual
{
  const auto imageSize = FSPInternalImageSizeFromCGSize(CGSizeMake(42.0f, 40.0f));
  const auto expectedImageSize = image::Size{.width = 42, .height = 40};

  XCTAssertTrue(imageSize == expectedImageSize);
}

- (void)testImageSizeToSizeIsNone
{
  const auto size = FSPSizeFromInternalImageSize(folly::none);

  XCTAssertTrue(CGSizeEqualToSize(size, CGSizeZero));
}

- (void)testImageSizeToSizeIsEqual
{
  const auto size = FSPSizeFromInternalImageSize(image::Size{.width = 42, .height = 40});
  const auto expectedSize = CGSizeMake(42, 40);

  XCTAssertTrue(CGSizeEqualToSize(size, expectedSize));
}

@end
