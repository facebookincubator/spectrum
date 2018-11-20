// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#define FSPObjectEqualObject(lhs, rhs) (lhs == rhs || (lhs != nil && [lhs isEqual:rhs]))

#include <folly/FixedString.h>

NS_INLINE NSString *FSPStringFromInternalFixedString(const folly::FixedString<10> &fixedString)
{
  return [NSString stringWithUTF8String:fixedString.cbegin()];
}

NS_INLINE folly::FixedString<10> FSPInternalFixedStringFromString(NSString *const string)
{
  return folly::FixedString<10>{string.UTF8String, string.length};
}
