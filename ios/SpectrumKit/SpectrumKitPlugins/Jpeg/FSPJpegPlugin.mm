// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPJpegPlugin.h"

#import <SpectrumKit/FSPPlugin_Private.h>

#include <spectrum/plugins/jpeg/LibJpegTranscodingPlugin.h>

using namespace facebook::spectrum;

@interface FSPJpegPlugin () <FSPPlugin_Private>
@end

@implementation FSPJpegPlugin

- (Plugin)makeInternalPlugin
{
  return plugins::jpeg::makeTranscodingPlugin();
}

@end
