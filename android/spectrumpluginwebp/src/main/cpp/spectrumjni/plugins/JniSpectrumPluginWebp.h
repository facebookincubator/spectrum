// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <fbjni/fbjni.h>

namespace facebook {
namespace spectrum {
namespace plugins {

class JSpectrumPluginWebp
    : public facebook::jni::HybridClass<JSpectrumPluginWebp> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/plugins/SpectrumPluginWebp;";

  static void registerNatives();

  static facebook::jni::local_ref<jhybriddata> initHybrid(
      facebook::jni::alias_ref<jclass>);

  jlong nativeCreatePlugin();

 private:
  friend HybridBase;
};

} // namespace plugins
} // namespace spectrum
} // namespace facebook