// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Plugin.h>
#include <vector>

namespace facebook {
namespace spectrum {
namespace core {

struct PluginAggregator : public Plugin {
  PluginAggregator(std::vector<Plugin>&& plugins);
};

} // namespace core
} // namespace spectrum
} // namespace facebook
