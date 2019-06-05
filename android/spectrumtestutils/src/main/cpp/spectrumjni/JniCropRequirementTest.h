// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <fbjni/fbjni.h>
#include <spectrumjni/requirements/JniCropRequirement.h>

namespace facebook {
namespace spectrum {
namespace requirements {

class JniCropRequirementTest
    : public facebook::jni::HybridClass<JniCropRequirementTest> {
 private:
  friend HybridBase;

 public:
  constexpr static auto kJavaDescriptor =
      "Lcom/facebook/spectrum/requirements/JniCropRequirementTest;";

  static facebook::jni::local_ref<jhybriddata> initHybrid(
      facebook::jni::alias_ref<jhybridobject>);

  std::string describe(
      facebook::jni::alias_ref<JCropRequirement> jCropRequirement);

  static void registerNatives();
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
