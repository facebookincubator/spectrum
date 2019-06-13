// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>
#import <UIKit/UIImage.h>

NS_ASSUME_NONNULL_BEGIN

#ifdef __cplusplus
extern "C" {
#endif

CGFloat FSPComputeSSIMFactorBetween(UIImage *left, UIImage *right);

#ifdef __cplusplus
}
#endif

NS_ASSUME_NONNULL_END
