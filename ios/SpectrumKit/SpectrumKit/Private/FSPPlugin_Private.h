// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import <Foundation/Foundation.h>

#import <SpectrumKit/FSPPlugin.h>

#include <spectrum/Plugin.h>

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(Plugin_Private)
@protocol FSPPlugin_Private <FSPPlugin>

- (facebook::spectrum::Plugin)makeInternalPlugin;

@end

NS_ASSUME_NONNULL_END
