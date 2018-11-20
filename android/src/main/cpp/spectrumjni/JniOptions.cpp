// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "JniOptions.h"

#include <spectrum/core/utils/Numeric.h>
#include <spectrumjni/JniConfiguration.h>
#include <spectrumjni/JniTransformations.h>
#include <spectrumjni/JniUtils.h>
#include <spectrumjni/image/JniMetadata.h>
#include <spectrumjni/image/JniSpecification.h>
#include <spectrumjni/requirements/JniEncode.h>

namespace facebook {
namespace spectrum {

//
// JOptions
//

facebook::jni::local_ref<JOptions> JOptions::fromNative(
    const Options& options) {
  return newInstance(
      jni::optionalToNullableJavaObject<
          decltype(options.encodeRequirement),
          requirements::JEncode>(options.encodeRequirement),
      JTransformations::fromNative(options.transformations),
      jni::optionalToNullableJavaObject<
          decltype(options.metadata),
          image::JMetadata>(options.metadata),
      JConfiguration::fromNative(options.configuration),
      jni::optionalToNullableJavaObject<
          decltype(options.outputPixelSpecificationRequirement),
          image::pixel::JSpecification>(
          options.outputPixelSpecificationRequirement));
}

folly::Optional<requirements::Encode> JOptions::encodeRequirement() const {
  static const auto field =
      javaClassStatic()->getField<requirements::JEncode::javaobject>(
          "encodeRequirement");
  const auto value = getFieldValue(field);
  return jni::nullableJavaObjectToOptional<requirements::Encode>(value);
}

Transformations JOptions::transformations() const {
  static const auto field =
      javaClassStatic()->getField<JTransformations::javaobject>(
          "transformations");
  return getFieldValue(field)->toNative();
}

folly::Optional<image::Metadata> JOptions::metadata() const {
  static const auto field =
      javaClassStatic()->getField<image::JMetadata::javaobject>("metadata");
  const auto value = getFieldValue(field);
  return jni::nullableJavaObjectToOptional<image::Metadata>(value);
}

Configuration JOptions::configuration() const {
  static const auto field =
      javaClassStatic()->getField<JConfiguration::javaobject>("configuration");
  const auto value = getFieldValue(field);
  return !value ? Configuration() : value->toNative();
}

folly::Optional<image::Specification> JOptions::inputImageSpecification()
    const {
  static const auto field =
      javaClassStatic()->getField<image::JSpecification::javaobject>(
          "imageSpecification");
  const auto value = getFieldValue(field);
  return jni::nullableJavaObjectToOptional<image::Specification>(value);
}

folly::Optional<image::pixel::Specification>
JOptions::outputPixelSpecification() const {
  static const auto field =
      javaClassStatic()->getField<image::pixel::JSpecification::javaobject>(
          "outputPixelSpecification");
  const auto value = getFieldValue(field);
  return jni::nullableJavaObjectToOptional<image::pixel::Specification>(value);
}

//
// JDecodeOptions
//

DecodeOptions JDecodeOptions::toNative() const {
  return DecodeOptions(
      transformations(), configuration(), outputPixelSpecification());
}

//
// JEncodeOptions
//

EncodeOptions JEncodeOptions::toNative() const {
  return EncodeOptions(
      *encodeRequirement(),
      transformations(),
      metadata(),
      configuration(),
      outputPixelSpecification());
}

//
// JTranscodeOptions
//

TranscodeOptions JTranscodeOptions::toNative() const {
  return TranscodeOptions(
      *encodeRequirement(),
      transformations(),
      metadata(),
      configuration(),
      outputPixelSpecification());
}

//
// JTransformOptions
//

TransformOptions JTransformOptions::toNative() const {
  return TransformOptions(
      transformations(), configuration(), outputPixelSpecification());
}

} // namespace spectrum
} // namespace facebook
