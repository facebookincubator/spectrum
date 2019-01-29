// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniConfiguration.h"

#include <spectrum/Configuration.h>

#include <spectrumjni/JniBaseTypes.h>
#include <spectrumjni/JniUtils.h>
#include <spectrumjni/image/JniImageHint.h>
#include <spectrumjni/image/JniSpecification.h>

#include <fbjni/fbjni.h>

#include <cassert>
#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {
namespace {

folly::Optional<image::ChromaSamplingMode>
_chromaSamplingModeOverrideToChromaSamplingMode(
    const folly::Optional<Configuration::General::ChromaSamplingModeOverride>&
        chromaSamplingModeOverride) {
  if (chromaSamplingModeOverride.hasValue()) {
    return Configuration::General::
        chromaSamplingModeOverrideToChromaSamplingMode(
            *chromaSamplingModeOverride);
  } else {
    return folly::none;
  }
}
} // namespace

facebook::jni::local_ref<JConfiguration> JConfiguration::fromNative(
    const Configuration& configuration) {
  return newInstance(
      // general
      jni::optionalToNullableJavaObject<
          decltype(configuration.general.optional_defaultBackgroundColor()),
          image::JColor>(
          configuration.general.optional_defaultBackgroundColor()),
      jni::optionalToNullableJavaObject<
          decltype(configuration.general.optional_interpretMetadata()),
          jni::JBoolean>(configuration.general.optional_interpretMetadata()),
      jni::optionalToNullableJavaObject<
          decltype(configuration.general.optional_samplingMethod()),
          JConfiguration::JSamplingMethod>(
          configuration.general.optional_samplingMethod()),
      jni::optionalToNullableJavaObject<
          decltype(configuration.general
                       .optional_propagateChromaSamplingModeFromSource()),
          jni::JBoolean>(configuration.general
                             .optional_propagateChromaSamplingModeFromSource()),
      jni::optionalToNullableJavaObject<
          folly::Optional<image::ChromaSamplingMode>,
          image::JChromaSamplingMode>(
          _chromaSamplingModeOverrideToChromaSamplingMode(
              configuration.general.optional_chromaSamplingModeOverride())),

      // jpeg
      jni::optionalToNullableJavaObject<
          decltype(configuration.jpeg.optional_useTrellis()),
          jni::JBoolean>(configuration.jpeg.optional_useTrellis()),
      jni::optionalToNullableJavaObject<
          decltype(configuration.jpeg.optional_useProgressive()),
          jni::JBoolean>(configuration.jpeg.optional_useProgressive()),
      jni::optionalToNullableJavaObject<
          decltype(configuration.jpeg.optional_useOptimizeScan()),
          jni::JBoolean>(configuration.jpeg.optional_useOptimizeScan()),
      jni::optionalToNullableJavaObject<
          decltype(configuration.jpeg.optional_useCompatibleDcScanOpt()),
          jni::JBoolean>(configuration.jpeg.optional_useCompatibleDcScanOpt()),
      jni::optionalToNullableJavaObject<
          decltype(configuration.jpeg.optional_usePsnrQuantTable()),
          jni::JBoolean>(configuration.jpeg.optional_usePsnrQuantTable()),

      // png
      jni::optionalToNullableJavaObject<
          decltype(configuration.png.optional_useInterlacing()),
          jni::JBoolean>(configuration.png.optional_useInterlacing()),
      jni::optionalToNullableJavaObject<
          decltype(configuration.png.optional_compressionLevel()),
          jni::JInteger>(configuration.png.optional_compressionLevel()),

      // Webp
      jni::optionalToNullableJavaObject<
          decltype(configuration.webp.optional_method()),
          jni::JInteger>(configuration.webp.optional_method()),
      jni::optionalToNullableJavaObject<
          decltype(configuration.webp.optional_imageHint()),
          image::JImageHint>(configuration.webp.optional_imageHint()));
}

Configuration JConfiguration::toNative() const {
  Configuration::General general;
  general.defaultBackgroundColor(defaultBackgroundColor());
  general.interpretMetadata(interpretMetadata());
  general.samplingMethod(samplingMethod());
  general.propagateChromaSamplingModeFromSource(
      propagateChromaSamplingModeFromSource());
  general.chromaSamplingModeOverride(chromaSamplingModeOverride());

  Configuration::Jpeg jpeg;
  jpeg.useTrellis(useTrellis());
  jpeg.useProgressive(useProgressive());
  jpeg.useOptimizeScan(useOptimizeScans());
  jpeg.useCompatibleDcScanOpt(useCompatibleDcScanOpt());
  jpeg.usePsnrQuantTable(usePsnrQuantTable());

  Configuration::Png png;
  png.useInterlacing(useInterlacing());
  png.compressionLevel(compressionLevel());

  Configuration::Webp webp;
  webp.method(method());
  webp.imageHint(imageHint());

  Configuration configuration;
  configuration.general = general;
  configuration.jpeg = jpeg;
  configuration.png = png;
  configuration.webp = webp;
  return configuration;
}

folly::Optional<image::Color> JConfiguration::defaultBackgroundColor() const {
  static const auto field =
      javaClassStatic()->getField<image::JColor::javaobject>(
          "defaultBackgroundColor");
  return jni::nullableJavaObjectToOptional<image::Color>(getFieldValue(field));
}

folly::Optional<bool> JConfiguration::interpretMetadata() const {
  static const auto field =
      javaClassStatic()->getField<jni::JBoolean::javaobject>(
          "interpretMetadata");
  return jni::nullableJavaObjectToOptional<bool>(getFieldValue(field));
}

folly::Optional<Configuration::General::SamplingMethod>
JConfiguration::samplingMethod() const {
  static const auto field =
      javaClassStatic()->getField<JConfiguration::JSamplingMethod::javaobject>(
          "samplingMethod");
  return jni::nullableJavaObjectToOptional<
      Configuration::General::SamplingMethod>(getFieldValue(field));
}

folly::Optional<bool> JConfiguration::propagateChromaSamplingModeFromSource()
    const {
  static const auto field =
      javaClassStatic()->getField<jni::JBoolean::javaobject>(
          "propagateChromaSamplingModeFromSource");
  return jni::nullableJavaObjectToOptional<bool>(getFieldValue(field));
}

folly::Optional<bool> JConfiguration::useTrellis() const {
  static const auto field =
      javaClassStatic()->getField<jni::JBoolean::javaobject>("useTrellis");
  return jni::nullableJavaObjectToOptional<bool>(getFieldValue(field));
}

folly::Optional<bool> JConfiguration::useProgressive() const {
  static const auto field =
      javaClassStatic()->getField<jni::JBoolean::javaobject>("useProgressive");
  return jni::nullableJavaObjectToOptional<bool>(getFieldValue(field));
}

folly::Optional<bool> JConfiguration::useOptimizeScans() const {
  static const auto field =
      javaClassStatic()->getField<jni::JBoolean::javaobject>("useOptimizeScan");
  return jni::nullableJavaObjectToOptional<bool>(getFieldValue(field));
}

folly::Optional<bool> JConfiguration::useCompatibleDcScanOpt() const {
  static const auto field =
      javaClassStatic()->getField<jni::JBoolean::javaobject>(
          "useCompatibleDcScanOpt");
  return jni::nullableJavaObjectToOptional<bool>(getFieldValue(field));
}

folly::Optional<Configuration::General::ChromaSamplingModeOverride>
JConfiguration::chromaSamplingModeOverride() const {
  static const auto field =
      javaClassStatic()->getField<image::JChromaSamplingMode::javaobject>(
          "chromaSamplingModeOverride");
  const auto chromaSamplingModeOverride =
      jni::nullableJavaObjectToOptional<image::ChromaSamplingMode>(
          getFieldValue(field));
  if (chromaSamplingModeOverride.hasValue()) {
    return Configuration::General::
        chromaSamplingModeOverrideFromChromaSamplingMode(
            *chromaSamplingModeOverride);
  } else {
    return folly::none;
  }
}

folly::Optional<bool> JConfiguration::usePsnrQuantTable() const {
  static const auto field =
      javaClassStatic()->getField<jni::JBoolean::javaobject>(
          "usePsnrQuantTable");
  return jni::nullableJavaObjectToOptional<bool>(getFieldValue(field));
}

folly::Optional<bool> JConfiguration::useInterlacing() const {
  static const auto field =
      javaClassStatic()->getField<jni::JBoolean::javaobject>("useInterlacing");
  return jni::nullableJavaObjectToOptional<bool>(getFieldValue(field));
}

folly::Optional<int> JConfiguration::compressionLevel() const {
  static const auto field =
      javaClassStatic()->getField<jni::JInteger::javaobject>(
          "compressionLevel");
  return jni::nullableJavaObjectToOptional<int>(getFieldValue(field));
}

folly::Optional<int> JConfiguration::method() const {
  static const auto field =
      javaClassStatic()->getField<jni::JInteger::javaobject>("webpMethod");
  return jni::nullableJavaObjectToOptional<int>(getFieldValue(field));
}

folly::Optional<Configuration::Webp::ImageHint> JConfiguration::imageHint()
    const {
  static const auto field =
      javaClassStatic()->getField<image::JImageHint::javaobject>(
          "webpImageHint");
  return jni::nullableJavaObjectToOptional<Configuration::Webp::ImageHint>(
      getFieldValue(field));
}

//
// JSamplingMethod
//

Configuration::General::SamplingMethod
JConfiguration::JSamplingMethod::toNative() const {
  return Configuration::General::makeSamplingMethodFromValue(value());
}

facebook::jni::local_ref<JConfiguration::JSamplingMethod>
JConfiguration::JSamplingMethod::fromNative(
    const Configuration::General::SamplingMethod& samplingMethod) {
  static const auto samplingMethodFrom =
      javaClassStatic()
          ->getStaticMethod<
              facebook::jni::local_ref<JConfiguration::JSamplingMethod>(jint)>(
              "from");
  return samplingMethodFrom(
      javaClassStatic(), static_cast<int>(samplingMethod));
}

int JConfiguration::JSamplingMethod::value() const {
  static const auto field = javaClassStatic()->getField<jint>("value");
  return getFieldValue(field);
}

} // namespace spectrum
} // namespace facebook
