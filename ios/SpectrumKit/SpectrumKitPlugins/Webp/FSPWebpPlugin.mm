// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#import "FSPWebpPlugin.h"
#import <SpectrumKit/FSPPlugin_Private.h>

#include <spectrum/plugins/webp/LibWebpDecodePlugin.h>
#include <spectrum/plugins/webp/LibWebpEncodePlugin.h>
#include <spectrum/core/PluginAggregator.h>

using namespace facebook::spectrum;

@interface FSPWebpPlugin () <FSPPlugin_Private>
@end

@implementation FSPWebpPlugin

- (Plugin)makeInternalPlugin
{
  auto encodePlugin = plugins::webp::makeEncodePlugin();
  encodePlugin.insert(plugins::webp::makeDecodePlugin());
  return encodePlugin;
}

@end
