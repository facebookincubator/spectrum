// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Color.h>
#include <spectrum/image/Specification.h>

#include <initializer_list>
#include <string>

#include <folly/Optional.h>
#include <folly/Range.h>

namespace facebook {
namespace spectrum {

#define SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(type, name, default) \
 private:                                                                    \
  folly::Optional<type> _##name;                                             \
                                                                             \
 public:                                                                     \
  type name() const {                                                        \
    return _##name.value_or(default);                                        \
  }                                                                          \
  folly::Optional<type> optional_##name() const {                            \
    return _##name;                                                          \
  }                                                                          \
  void name(const folly::Optional<type>& name) {                             \
    _##name = name;                                                          \
  }

bool configurationCurrentPlatformValue(const bool android, const bool ios);

/**
 * The Configuration class defines configuration parameters that can be used
 * to tweak low level options of how an operation is performed. Each of these
 * configurations have a default value which can be overridden at two levels:
 *
 * - At the Spectrum instance which can be created with a custom configuration
 * object.
 * - At the operation which can provide a configuration override through its
 * options.
 *
 * See the external documentation for details on these options.
 */
struct Configuration {
  //
  // General image processing parameters
  //
  struct General {
    enum class ChromaSamplingModeOverride {
      None = 0,
      S444,
      S420,
      S422,
      S411,
      S440,
    };

    static std::string chromaSamplingModeOverrideStringFromValue(
        const ChromaSamplingModeOverride chromaSamplingModeOverride);

    static ChromaSamplingModeOverride chromaSamplingModeOverrideFromValue(
        const int value);

    static ChromaSamplingModeOverride
    chromaSamplingModeOverrideFromChromaSamplingMode(
        const image::ChromaSamplingMode chromaSamplingMode);

    static folly::Optional<image::ChromaSamplingMode>
    chromaSamplingModeOverrideToChromaSamplingMode(
        const ChromaSamplingModeOverride chromaSamplingModeOverride);

    static ChromaSamplingModeOverride
    chromaSamplingModeOverrideFromChromaSamplingMode(
        const folly::Optional<image::ChromaSamplingMode>& chromaSamplingMode);

    enum class SamplingMethod : std::uint8_t {
      Bicubic = 1,
      MagicKernel = 2,
    };

    static SamplingMethod makeSamplingMethodFromValue(const int value);

    static std::string samplingMethodStringFromValue(
        const SamplingMethod samplingMethod);

    /**
     * General: Default background color to merge transparent images onto.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        image::Color,
        defaultBackgroundColor,
        image::Color({255, 255, 255}));

    /**
     * General: Whether to read and interpret metadata such as EXIF for JPEG
     * files.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        bool,
        interpretMetadata,
        true);

    /**
     * General: Whether to propagate sampling mode from the source image. If
     * enabled, a source image with S444 will make the output default to S444 if
     * the codecs support it.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        bool,
        propagateChromaSamplingModeFromSource,
        true);

    /**
     * General: The sampling mode to use when scaling the image
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        SamplingMethod,
        samplingMethod,
        SamplingMethod::MagicKernel);

    /*
     * An exception will be throw if set but not supported by the target format.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        ChromaSamplingModeOverride,
        chromaSamplingModeOverride,
        ChromaSamplingModeOverride::None);

    void merge(const General& rhs);
    bool operator==(const General& rhs) const;
  } general;

  //
  // MozJpeg / LibJpeg parameters
  //
  struct Jpeg {
    /**
     * Whether to use trellis optimization when encoding JPEG images.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(bool, useTrellis, true);

    /**
     * Whether to save JPEG images in progressive scan encoding.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        bool,
        useProgressive,
        configurationCurrentPlatformValue(true, false));

    /**
     * Whether to use scan optimization when encoding JPEG images.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        bool,
        useOptimizeScan,
        false);

    /**
     * Whether to use compatible DC scan configuration (keeping all DC
     * components in the first scan) when encoding JPEG images.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        bool,
        useCompatibleDcScanOpt,
        configurationCurrentPlatformValue(true, false));

    /**
     * Whether to use an alternative quantization table optimized for PSNR when
     * encoding JPEG images.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        bool,
        usePsnrQuantTable,
        false);

    /**
     * Whether to propagate sampling mode from the source image. If enabled, a
     * source image with S444 will make the output default to S444.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        bool,
        propagateSamplingModeFromSource,
        true);

    void merge(const Jpeg& rhs);
    bool operator==(const Jpeg& rhs) const;
  } jpeg;

  //
  // LibPng parameters
  //
  struct Png {
    using CompressionLevel = int;
    static const CompressionLevel CompressionLevelNone;
    static const CompressionLevel CompressionLevelBestSpeed;
    static const CompressionLevel CompressionLevelBestCompression;
    static const CompressionLevel CompressionLevelDefault;

    /**
     * Whether to save PNG images with interlaced encoding.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        bool,
        useInterlacing,
        false);

    /**
     * The compression level that is used by zlib to determine
     * how much time to spend on trying to compress the image data.
     * 0 is for not using compression at all and 9 is for the best compression.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        CompressionLevel,
        compressionLevel,
        -1);

    void merge(const Png& rhs);
    bool operator==(const Png& rhs) const;
  } png;

  //
  // LibWebp parameters
  //
  struct Webp {
    enum class ImageHint : std::uint8_t {
      /** Default preset */
      Default = 0,
      /** General indoor digital picture (e.g. portrait) */
      Picture = 1,
      /** General outdoor digital picture (e.g. landscape) */
      Photo = 2,
      /** Graphical image */
      Graph = 3,
    };

    /**
     * Compression quality/speed tradeoff where 0 is the fastest and 6 is the
     * slowest/best compression.
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(int, method, 3);

    /**
     * Provides an optional ImageHint to the webp encoder
     */
    SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS(
        ImageHint,
        imageHint,
        ImageHint::Default);

    void merge(const Webp& rhs);
    bool operator==(const Webp& rhs) const;

    static std::string imageHintStringFromValue(const ImageHint imageHint);
    static ImageHint makeImageHintFromValue(const int value);
  } webp;

  /**
   * Merges two configuration together.
   *
   * @param rhs The configuration to merge. rhs has precedence over this.
   */
  void merge(const Configuration& rhs);

  /**
   * Merges two configuration together.
   *
   * @param rhs The configuration to merge. this has precedence over rhs.
   */
  void mergeInto(const Configuration& rhs);

  bool operator==(const Configuration& rhs) const;
  bool operator!=(const Configuration& rhs) const;
};

#define SPECTRUM_UNREACHABLE_CONFIGURATION_WEBP_IMAGE_HINT(imageHint)  \
  SPECTRUM_UNREACHABLE_STRING(                                         \
      folly::StringPiece{"unreachable_configuration_webp_image_hint"}, \
      Configuration::Webp::imageHintStringFromValue(imageHint))

#define SPECTRUM_UNREACHABLE_CONFIGURATION_CHROMA_SAMPLING_MODE_OVERRIDE( \
    chromaSamplingModeOverride)                                           \
  SPECTRUM_UNREACHABLE_STRING(                                            \
      folly::StringPiece{                                                 \
          "unreachable_configuration_chroma_sampling_mode_override"},     \
      Configuration::General::chromaSamplingModeOverrideStringFromValue(  \
          chromaSamplingModeOverride))

#define SPECTRUM_UNREACHABLE_CONFIGURATION_SAMPLING_METHOD(samplingMethod) \
  SPECTRUM_UNREACHABLE_STRING(                                             \
      folly::StringPiece{"unreachable_configuration_sampling_method"},     \
      Configuration::General::samplingMethodStringFromValue(samplingMethod))

#undef SPECTRUM_CONFIGURATION_MAKE_PROPERTY_W_DEFAULTS

} // namespace spectrum
} // namespace facebook
