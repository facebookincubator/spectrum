// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import "FSPPrivate.h"

@interface FSPInternalTests : XCTestCase
@end

@implementation FSPInternalTests

static const auto kFixedString3 = folly::FixedString<10>{"012"};
static const auto kFixedString10 = folly::FixedString<10>{"0123456789"};
static const NSString *kString3 = @"012";
static const NSString *kString10 = @"0123456789";

- (void)testString10ToInternalFixedString
{
  const auto string = FSPInternalFixedStringFromString(@"0123456789");

  XCTAssertEqual(kFixedString10, string);
}

- (void)testString3ToInternalFixedString
{
  const auto string = FSPInternalFixedStringFromString(@"012");

  XCTAssertEqual(kFixedString3, string);
}

- (void)testInternalFixedString3ToString
{
  const auto string = FSPStringFromInternalFixedString({"012"});

  XCTAssertEqualObjects(kString3, string);
}

- (void)testInternalFixedString10ToString
{
  const auto string = FSPStringFromInternalFixedString({"0123456789"});

  XCTAssertEqualObjects(kString10, string);
}

@end
