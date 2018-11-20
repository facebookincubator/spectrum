// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Pixel.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>
#include <string>

namespace facebook {
namespace spectrum {
namespace image {
namespace pixel {

class JAlphaInfo : public facebook::jni::JavaClass<JAlphaInfo> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImagePixelSpecification$AlphaInfo;";

  image::pixel::AlphaInfo toNative() const;
  static facebook::jni::local_ref<JAlphaInfo> fromNative(
      const image::pixel::AlphaInfo& alphaInfo);

 private:
  int value() const;
};

class JColorModel : public facebook::jni::JavaClass<JColorModel> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImagePixelSpecification$ColorModel;";

  image::pixel::ColorModel toNative() const;
  static facebook::jni::local_ref<JColorModel> fromNative(
      const image::pixel::ColorModel& colorModel);

 private:
  std::string identifier() const;
  int numberOfColorComponents() const;
  bool supportsExtraAlphaChannel() const;
};

class JComponentsOrder : public facebook::jni::JavaClass<JComponentsOrder> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImagePixelSpecification$ComponentsOrder;";

  image::pixel::ComponentsOrder toNative() const;
  static facebook::jni::local_ref<JComponentsOrder> fromNative(
      const image::pixel::ComponentsOrder& componentsOrder);

 private:
  int value() const;
};

class JSpecification : public facebook::jni::JavaClass<JSpecification> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/image/ImagePixelSpecification;";

  image::pixel::Specification toNative() const;
  static facebook::jni::local_ref<JSpecification> fromNative(
      const image::pixel::Specification& pixelSpecification);

 private:
  ColorModel colorModel() const;
  int bytesPerPixel() const;
  AlphaInfo alphaInfo() const;
  ComponentsOrder componentsOrder() const;
};

} // namespace pixel
} // namespace image
} // namespace spectrum
} // namespace facebook
