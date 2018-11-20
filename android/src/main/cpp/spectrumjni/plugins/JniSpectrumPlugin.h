// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Plugin.h>

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {
namespace plugins {

class JSpectrumPlugin : public facebook::jni::JavaClass<JSpectrumPlugin> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/plugins/SpectrumPlugin;";

  jlong getPlugin() const;
};

} // namespace plugins
} // namespace spectrum
} // namespace facebook