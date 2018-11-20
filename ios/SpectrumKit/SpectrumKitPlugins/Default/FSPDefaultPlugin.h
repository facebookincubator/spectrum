// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/SpectrumKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPSpectrum (DefaultPlugin)

@property (nonatomic, strong, readonly, class) FSPSpectrum *sharedInstance NS_SWIFT_NAME(shared);

@end

NS_ASSUME_NONNULL_END
