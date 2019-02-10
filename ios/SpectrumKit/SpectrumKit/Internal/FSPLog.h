// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <FBLog/FBLog.h>
#import <FBDefines/FBDefines.h>

#define FSPReportMustFixIfNot(condition, message) FBReportMustFixIfNot(condition, message)
#define FSPReportMustFixIfNil(condition, message) FBReportMustFixIfNot(condition, message)
#define FSPReportMustFixIf(condition, message) FBReportMustFixIf(condition, message)
#define FSPReportMustFixIfFalse(condition, message) FBReportMustFixIfFalse(condition, message)
#define FSPCReportMustFix(message, desc) FBCReportMustFix(message, desc)

#define FSP_LINK_REQUIRE(name) FB_LINK_REQUIRE_CATEGORY(name)
#define FSP_LINKABLE(name) FB_LINKABLE(name)
