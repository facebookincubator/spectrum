// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#include "TestData.h"

#include <stdexcept>
#include <string>

@interface FBTestDataBundleLocator : NSObject
@end

@implementation FBTestDataBundleLocator
@end

namespace facebook {
namespace spectrum {
namespace testdata {

std::string Path::normalized() const {
  @autoreleasepool {
    const auto filepathString =
        [NSString stringWithUTF8String:path.toString().c_str()];
    const auto URL = [NSURL fileURLWithPath:filepathString];
    const auto bundle = [NSBundle bundleForClass:FBTestDataBundleLocator.class];
    const auto finalPath =
        [bundle pathForResource:URL.URLByDeletingPathExtension.lastPathComponent
                         ofType:URL.pathExtension];

    if (finalPath == nil) {
      throw std::runtime_error("Could not find testdata ");
    } else {
      return finalPath.UTF8String;
    }
  }
}
}
}
} // namespace
