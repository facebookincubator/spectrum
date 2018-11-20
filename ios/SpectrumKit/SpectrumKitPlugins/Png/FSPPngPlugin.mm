// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPPngPlugin.h"

#import <SpectrumKit/FSPPlugin_Private.h>

#include <spectrum/plugins/png/LibPngTranscodingPlugin.h>

using namespace facebook::spectrum;

@interface FSPPngPlugin () <FSPPlugin_Private>
@end

@implementation FSPPngPlugin

- (Plugin)makeInternalPlugin
{
  return plugins::png::makeTranscodingPlugin();
}

@end
