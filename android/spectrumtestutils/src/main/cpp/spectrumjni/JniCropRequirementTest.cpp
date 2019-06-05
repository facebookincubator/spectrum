// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniCropRequirementTest.h"

namespace facebook {
namespace spectrum {
namespace requirements {

facebook::jni::local_ref<JniCropRequirementTest::jhybriddata>
JniCropRequirementTest::initHybrid(
    facebook::jni::alias_ref<JniCropRequirementTest::jhybridobject>) {
  return makeCxxInstance();
}

std::string JniCropRequirementTest::describe(
    facebook::jni::alias_ref<JCropRequirement> jCropRequirement) {
  const auto& twin = jCropRequirement->toNative();
  return JCropRequirement::toString(twin);
}

void JniCropRequirementTest::registerNatives() {
  registerHybrid({
      makeNativeMethod("initHybrid", JniCropRequirementTest::initHybrid),
      makeNativeMethod("describe", JniCropRequirementTest::describe),
  });
}

} // namespace requirements
} // namespace spectrum
} // namespace facebook
