// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/requirements/Crop.h>
#include <spectrum/requirements/CropAbsoluteToOrigin.h>
#include <spectrum/requirements/CropRelativeToOrigin.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>
#include <string>

namespace facebook {
namespace spectrum {
namespace requirements {

class JCropRequirement : public facebook::jni::JavaClass<JCropRequirement> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/requirements/CropRequirement;";

  Crop toNative() const;
  static std::string toString(const Crop& crop);

 protected:
  bool mustBeExact() const;
};

class JAbsoluteCropRequirement
    : public facebook::jni::
          JavaClass<JAbsoluteCropRequirement, JCropRequirement> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/requirements/CropRequirement$CropAbsoluteToOriginRequirement;";

  Crop toNative() const;

 protected:
  int top() const;
  int left() const;
  int bottom() const;
  int right() const;
};

class JRelativeCropRequirement
    : public facebook::jni::
          JavaClass<JRelativeCropRequirement, JCropRequirement> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/requirements/CropRequirement$CropRelativeToOriginRequirement;";

  Crop toNative() const;

 protected:
  float top() const;
  float left() const;
  float bottom() const;
  float right() const;
};

} // namespace requirements
} // namespace spectrum
} // namespace facebook
