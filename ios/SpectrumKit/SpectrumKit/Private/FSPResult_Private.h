// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPResult.h>
#include <spectrum/Result.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPResult (Private)

- (instancetype)initWithInternalTranscodeResult:(const facebook::spectrum::Result &)result;

@end

NS_ASSUME_NONNULL_END
