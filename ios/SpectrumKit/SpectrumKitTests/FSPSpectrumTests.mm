// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>

#import <SpectrumKit/FSPDefaultPlugin.h>

@interface FSPSpectrumTests : XCTestCase
@end

@implementation FSPSpectrumTests

- (void)testSharedInstance
{
  XCTAssertNotNil(FSPSpectrum.sharedInstance);
  XCTAssertEqual(FSPSpectrum.sharedInstance, FSPSpectrum.sharedInstance);
}

@end
