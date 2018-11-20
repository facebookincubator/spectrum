// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPOptions.h>

#include <spectrum/Options.h>

NS_ASSUME_NONNULL_BEGIN

@interface FSPEncodeOptions (Private)

- (facebook::spectrum::EncodeOptions)makeInternalEncodeOptions;

@end

@interface FSPTranscodeOptions (Private)

- (facebook::spectrum::TranscodeOptions)makeInternalTranscodeOptions;

@end

NS_ASSUME_NONNULL_END
