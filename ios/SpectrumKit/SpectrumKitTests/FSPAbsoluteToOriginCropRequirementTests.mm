// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPAbsoluteToOriginCropRequirement.h>
#import <SpectrumKit/FSPCropRequirementProviding_Private.h>
#include <spectrum/requirements/CropAbsoluteToOrigin.h>

using namespace facebook::spectrum;

@interface FSPAbsoluteToOriginCropRequirementTests : XCTestCase
@end

@implementation FSPAbsoluteToOriginCropRequirementTests

static FSPAbsoluteToOriginCropRequirementValues kDefaultTestValues = {.left = 1, .top = 2, .right = 3, .bottom = 4};

- (void)testConformsToInternalCropRequirementProvidingProtocol
{
  const auto object = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:kDefaultTestValues];

  XCTAssertTrue([object conformsToProtocol:@protocol(FSPCropRequirementProviding_Private)]);
}

- (void)testInternalTypeConversion
{
  const auto object = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:kDefaultTestValues
                                                                                        enforcement:FSPCropRequirementEnforcementApproximate];
  const auto internalObject = [(id<FSPCropRequirementProviding_Private>)object makeInternalCropRequirement];
  const auto expectedInternalObject = requirements::CropAbsoluteToOrigin({
    .left = 1, .top = 2, .right = 3, .bottom = 4
  }, false);

  XCTAssertTrue(internalObject == expectedInternalObject);
}

- (void)testEnforcement
{
  const auto object = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:kDefaultTestValues
                                                                                        enforcement:FSPCropRequirementEnforcementApproximate];

  XCTAssertEqual(object.enforcement, FSPCropRequirementEnforcementApproximate);
}

#pragma mark - Equality

- (void)testIsEqual
{
  const auto object = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:kDefaultTestValues];
  const auto object2 = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:kDefaultTestValues];

  XCTAssertEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentSize
{
  const auto object = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:kDefaultTestValues];
  const auto object2 = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:{.right = 2, .bottom = 1}];

  XCTAssertNotEqualObjects(object, object2);
}

- (void)testIsNotEqualOnDifferentEnforcement
{
  const auto object = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:kDefaultTestValues
                                                                                        enforcement:FSPCropRequirementEnforcementMustBeExact];
  const auto object2 = [FSPAbsoluteToOriginCropRequirement absoluteToOriginCropRequirementWithValues:kDefaultTestValues
                                                                                         enforcement:FSPCropRequirementEnforcementApproximate];

  XCTAssertNotEqualObjects(object, object2);
}

@end
