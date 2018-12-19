// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIImage.h>

#import <FBBaseLite/FBBaseDefines.h>

NS_ASSUME_NONNULL_BEGIN

FB_EXTERN_C_BEGIN

CGFloat FSPComputeSSIMFactorBetween(UIImage *left, UIImage *right);

FB_EXTERN_C_END

NS_ASSUME_NONNULL_END
