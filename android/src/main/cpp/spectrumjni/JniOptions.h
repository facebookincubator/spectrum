// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Options.h>

#include <spectrumjni/JniConfiguration.h>
#include <spectrumjni/JniTransformations.h>
#include <spectrumjni/image/JniMetadata.h>
#include <spectrumjni/image/JniPixel.h>
#include <spectrumjni/image/JniSpecification.h>
#include <spectrumjni/requirements/JniEncode.h>

#include <fbjni/fbjni.h>

#include <cstddef>
#include <exception>

namespace facebook {
namespace spectrum {

class JOptions : public facebook::jni::JavaClass<JOptions> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/options/Options;";

  static facebook::jni::local_ref<JOptions> fromNative(const Options& options);

 protected:
  folly::Optional<requirements::Encode> encodeRequirement() const;
  Transformations transformations() const;
  folly::Optional<image::Metadata> metadata() const;
  Configuration configuration() const;
  folly::Optional<image::Specification> inputImageSpecification() const;
  folly::Optional<image::pixel::Specification> outputPixelSpecification() const;
};

class JDecodeOptions
    : public facebook::jni::JavaClass<JDecodeOptions, JOptions> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/options/DecodeOptions;";

  DecodeOptions toNative() const;
};

class JEncodeOptions
    : public facebook::jni::JavaClass<JEncodeOptions, JOptions> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/options/EncodeOptions;";

  EncodeOptions toNative() const;
};

class JTranscodeOptions
    : public facebook::jni::JavaClass<JTranscodeOptions, JOptions> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/options/TranscodeOptions;";

  TranscodeOptions toNative() const;
};

class JTransformOptions
    : public facebook::jni::JavaClass<JTransformOptions, JOptions> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/options/TransformOptions;";

  TransformOptions toNative() const;
};

} // namespace spectrum
} // namespace facebook
