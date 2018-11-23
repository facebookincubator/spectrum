// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#define FSPReportMustFixIfNot(condition, message) NSAssert(condition, message)
#define FSPReportMustFixIfNil(condition, message) NSAssert(condition != nil, message)
#define FSPReportMustFixIf(condition, message) NSAssert(!(condition), message)
#define FSPCReportMustFix(message, desc) NSCAssert1(false, message, desc)
#define FSPReportMustFixIfFalse(condition, message) NSCAssert(condition, message)

#define FSP_LINK_REQUIRE(name)
#define FSP_LINKABLE(name)
