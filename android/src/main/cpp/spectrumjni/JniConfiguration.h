// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/Configuration.h>

#include <spectrumjni/JniBaseTypes.h>

#include <fbjni/fbjni.h>

#include <cstddef>

namespace facebook {
namespace spectrum {

class JConfiguration : public facebook::jni::JavaClass<JConfiguration> {
 public:
  static constexpr const char* kJavaDescriptor =
      "Lcom/facebook/spectrum/Configuration;";

  static facebook::jni::local_ref<JConfiguration> fromNative(
      const Configuration& configuration);
  Configuration toNative() const;

  class JSamplingMethod : public facebook::jni::JavaClass<JSamplingMethod> {
   public:
    static constexpr const char* kJavaDescriptor =
        "Lcom/facebook/spectrum/Configuration$SamplingMethod;";

    Configuration::General::SamplingMethod toNative() const;
    static facebook::jni::local_ref<JSamplingMethod> fromNative(
        const Configuration::General::SamplingMethod& samplingMethod);

   private:
    int value() const;
  };

 private:
  folly::Optional<image::Color> defaultBackgroundColor() const;
  folly::Optional<bool> interpretMetadata() const;
  folly::Optional<bool> propagateChromaSamplingModeFromSource() const;
  folly::Optional<Configuration::General::SamplingMethod> samplingMethod()
      const;
  folly::Optional<bool> propagateSamplingModeFromSource() const;
  folly::Optional<bool> useTrellis() const;
  folly::Optional<bool> useProgressive() const;
  folly::Optional<bool> useOptimizeScans() const;
  folly::Optional<bool> useCompatibleDcScanOpt() const;
  folly::Optional<Configuration::General::ChromaSamplingModeOverride>
  chromaSamplingModeOverride() const;
  folly::Optional<bool> usePsnrQuantTable() const;
  folly::Optional<bool> useInterlacing() const;
  folly::Optional<int> compressionLevel() const;
  folly::Optional<int> method() const;
  folly::Optional<Configuration::Webp::ImageHint> imageHint() const;
};

} // namespace spectrum
} // namespace facebook
