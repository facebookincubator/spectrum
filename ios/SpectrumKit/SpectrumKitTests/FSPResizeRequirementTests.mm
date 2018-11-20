// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPResizeRequirement_Private.h>

using namespace facebook::spectrum;

@interface FSPResizeRequirementTests : XCTestCase
@end

@implementation FSPResizeRequirementTests

static const CGSize kDefaultSize = { .width = 42.0f, .height = 42.0f};
static const image::Size kDefaultInternalSize = { .width = 42, .height = 42};

- (void)testDefaultValues
{
  const auto objectExact = [FSPResizeRequirement resizeRequirementExactWithTargetSize:kDefaultSize];

  XCTAssertEqual(objectExact.mode, FSPResizeRequirementModeExact);
}

#pragma mark - Mode

- (void)testResizeRequirementExact
{
  const auto object = [[FSPResizeRequirement alloc] initWithMode:FSPResizeRequirementModeExact targetSize:kDefaultSize];
  const auto expectedInternalObject = requirements::Resize{requirements::Resize::Mode::Exact, kDefaultInternalSize};

  XCTAssertTrue([object makeInternalResizeRequirement] == expectedInternalObject);
}

- (void)testResizeRequirementExactOrSmaller
{
  const auto object = [[FSPResizeRequirement alloc] initWithMode:FSPResizeRequirementModeExactOrSmaller targetSize:kDefaultSize];
  const auto expectedInternalObject = requirements::Resize{requirements::Resize::Mode::ExactOrSmaller, kDefaultInternalSize};

  XCTAssertTrue([object makeInternalResizeRequirement] == expectedInternalObject);
}

#pragma mark - Equality

- (void)testIsEqual
{
  const auto object = [FSPResizeRequirement resizeRequirementExactWithTargetSize:kDefaultSize];
  const auto object2 = [FSPResizeRequirement resizeRequirementExactWithTargetSize:kDefaultSize];

  XCTAssertEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentMode
{
  const auto object = [FSPResizeRequirement resizeRequirementExactWithTargetSize:kDefaultSize];
  const auto object2 = [[FSPResizeRequirement alloc] initWithMode:FSPResizeRequirementModeExactOrLarger targetSize:kDefaultSize];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentTargetSize
{
  const auto object = [FSPResizeRequirement resizeRequirementExactWithTargetSize:kDefaultSize];
  const auto object2 = [FSPResizeRequirement resizeRequirementExactWithTargetSize:CGSizeZero];

  XCTAssertNotEqualObjects(object, object2);
}

@end
