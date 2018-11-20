// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniPixel.h"

#include <spectrum/Options.h>

#include <fbjni/fbjni.h>
#include <folly/FixedString.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace pixel {

//
// JAlphaInfo
//

image::pixel::AlphaInfo JAlphaInfo::toNative() const {
  return image::pixel::alphaInfoFromValue(value());
}

facebook::jni::local_ref<JAlphaInfo> JAlphaInfo::fromNative(
    const image::pixel::AlphaInfo& alphaInfo) {
  static const auto method =
      javaClassStatic()
          ->getStaticMethod<facebook::jni::local_ref<JAlphaInfo>(jint)>("from");
  return method(javaClassStatic(), static_cast<int>(alphaInfo));
}

int JAlphaInfo::value() const {
  static const auto field = javaClassStatic()->getField<jint>("value");
  return getFieldValue(field);
}

//
// JColorModel
//

image::pixel::ColorModel JColorModel::toNative() const {
  const auto identifierString = identifier();
  return image::pixel::ColorModel(
      folly::FixedString<10>(identifierString.c_str(), identifierString.size()),
      numberOfColorComponents(),
      supportsExtraAlphaChannel());
}

facebook::jni::local_ref<JColorModel> JColorModel::fromNative(
    const image::pixel::ColorModel& colorModel) {
  static const auto method =
      javaClassStatic()
          ->getStaticMethod<facebook::jni::local_ref<JColorModel>(
              facebook::jni::local_ref<facebook::jni::JString>,
              jint,
              jboolean)>("from");

  const auto identifier = facebook::jni::make_jstring(colorModel.identifier);
  return method(
      javaClassStatic(),
      identifier,
      colorModel.numberOfColorComponents,
      colorModel.supportsExtraAlphaChannel);
}

std::string JColorModel::identifier() const {
  static const auto field = javaClassStatic()->getField<jstring>("identifier");
  return getFieldValue(field)->toStdString();
}

int JColorModel::numberOfColorComponents() const {
  static const auto field =
      javaClassStatic()->getField<jint>("numberOfColorComponents");
  return getFieldValue(field);
}

bool JColorModel::supportsExtraAlphaChannel() const {
  static const auto field =
      javaClassStatic()->getField<jboolean>("supportsExtraAlphaChannel");
  return getFieldValue(field);
}

//
// JComponentsOrder
//

image::pixel::ComponentsOrder JComponentsOrder::toNative() const {
  return image::pixel::makeComponentsOrderFromValue(value());
}

facebook::jni::local_ref<JComponentsOrder> JComponentsOrder::fromNative(
    const image::pixel::ComponentsOrder& alphaInfo) {
  static const auto method =
      javaClassStatic()
          ->getStaticMethod<facebook::jni::local_ref<JComponentsOrder>(jint)>(
              "from");
  return method(javaClassStatic(), static_cast<int>(alphaInfo));
}

int JComponentsOrder::value() const {
  static const auto field = javaClassStatic()->getField<jint>("value");
  return getFieldValue(field);
}

//
// JSpecification
//

image::pixel::Specification JSpecification::toNative() const {
  return image::pixel::Specification(
      colorModel(), bytesPerPixel(), alphaInfo(), componentsOrder());
}

facebook::jni::local_ref<JSpecification> JSpecification::fromNative(
    const image::pixel::Specification& pixelSpecification) {
  static const auto method =
      javaClassStatic()
          ->getStaticMethod<facebook::jni::local_ref<JSpecification>(
              facebook::jni::local_ref<JColorModel>,
              jint,
              facebook::jni::local_ref<JAlphaInfo>,
              facebook::jni::local_ref<JComponentsOrder>)>("from");

  return method(
      javaClassStatic(),
      JColorModel::fromNative(pixelSpecification.colorModel),
      pixelSpecification.bytesPerPixel,
      JAlphaInfo::fromNative(pixelSpecification.alphaInfo),
      JComponentsOrder::fromNative(pixelSpecification.componentsOrder));
}

ColorModel JSpecification::colorModel() const {
  static const auto field =
      javaClassStatic()->getField<JColorModel::javaobject>("colorModel");
  return getFieldValue(field)->toNative();
}

int JSpecification::bytesPerPixel() const {
  static const auto field = javaClassStatic()->getField<jint>("bytesPerPixel");
  return getFieldValue(field);
}

AlphaInfo JSpecification::alphaInfo() const {
  static const auto field =
      javaClassStatic()->getField<JAlphaInfo::javaobject>("alphaInfo");
  return getFieldValue(field)->toNative();
}

ComponentsOrder JSpecification::componentsOrder() const {
  static const auto field =
      javaClassStatic()->getField<JComponentsOrder::javaobject>(
          "componentsOrder");
  return getFieldValue(field)->toNative();
}

} // namespace pixel
} // namespace image
} // namespace spectrum
} // namespace facebook
