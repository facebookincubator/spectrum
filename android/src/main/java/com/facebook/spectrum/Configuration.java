/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.image.ImageChromaSamplingMode;
import com.facebook.spectrum.image.ImageColor;
import com.facebook.spectrum.utils.Preconditions;
import javax.annotation.Nullable;
import javax.annotation.concurrent.Immutable;

/**
 * The Configuration class defines configuration parameters that can be used to tweak low level
 * options of how an operation is performed. Each of these configurations have a default value which
 * can be overridden at two levels:
 *
 * <ul>
 *   <li>At the Spectrum instance which can be created with a custom configuration object.
 *   <li>At the operation which can provide a configuration override through its options.
 * </ul>
 *
 * <p>See the external documentation for details on these options.
 */
@DoNotStrip
public class Configuration {
  /** General: Default background color to merge transparent images onto. */
  @DoNotStrip @Nullable public final ImageColor defaultBackgroundColor;

  /** General: Whether to read and interpret metadata such as EXIF for JPEG files. */
  @DoNotStrip @Nullable public final Boolean interpretMetadata;

  /** General: The sampling mode to use when scaling the image. */
  @DoNotStrip @Nullable public final SamplingMethod samplingMethod;

  /**
   * General: The {@link com.facebook.spectrum.image.ImageChromaSamplingMode} to use when encoding.
   */
  @DoNotStrip @Nullable public final ImageChromaSamplingMode chromaSamplingModeOverride;

  /**
   * General: Whether to propagate sampling mode from the source image. If enabled, a source image
   * with S444 will make the output default to S444.
   */
  @DoNotStrip @Nullable public final Boolean propagateChromaSamplingModeFromSource;

  /** Jpeg: Whether to use trellis optimization when encoding JPEG images. */
  @DoNotStrip @Nullable public final Boolean useTrellis;

  /** Jpeg: Whether to save JPEG images in progressive scan encoding. */
  @DoNotStrip @Nullable public final Boolean useProgressive;

  /** Jpeg: Whether to use scan optimization when encoding JPEG images. */
  @DoNotStrip @Nullable public final Boolean useOptimizeScan;

  /**
   * Jpeg: Whether to use compatible DC scan configuration (keeping all DC components in the first
   * scan) when encoding JPEG images.
   */
  @DoNotStrip @Nullable public final Boolean useCompatibleDcScanOpt;

  /**
   * Jpeg: Whether to use an alternative quantization table optimized for PSNR when encoding JPEG
   * images.
   */
  @DoNotStrip @Nullable public final Boolean usePsnrQuantTable;

  /** Png: Whether to save PNG images with interlaced encoding. */
  @DoNotStrip @Nullable public final Boolean useInterlacing;

  /**
   * Png: The compression level that is used by zlib to determine how much time to spend on trying
   * to compress the image data. 0 is for not using compression at all and 9 is for the best
   * compression.
   */
  @DoNotStrip @Nullable public final Integer compressionLevel;

  /**
   * Webp: Compression quality/speed tradeoff where 0 is the fastest and 6 is the slowest/best
   * compression.
   */
  @DoNotStrip @Nullable public final Integer webpMethod;

  /** Webp: Provides an optional {@link ImageHint} to the webp encoder */
  @DoNotStrip @Nullable public final ImageHint webpImageHint;

  @DoNotStrip
  private Configuration(
      @Nullable ImageColor defaultBackgroundColor,
      @Nullable Boolean interpretMetadata,
      @Nullable SamplingMethod samplingMethod,
      @Nullable Boolean propagateChromaSamplingModeFromSource,
      @Nullable ImageChromaSamplingMode chromaSamplingModeOverride,
      @Nullable Boolean useTrellis,
      @Nullable Boolean useProgressive,
      @Nullable Boolean useOptimizeScan,
      @Nullable Boolean useCompatibleDcScanOpt,
      @Nullable Boolean usePsnrQuantTable,
      @Nullable Boolean useInterlacing,
      @Nullable Integer compressionLevel,
      @Nullable Integer webpMethod,
      @Nullable ImageHint webpImageHint) {
    this.defaultBackgroundColor = defaultBackgroundColor;
    this.interpretMetadata = interpretMetadata;
    this.propagateChromaSamplingModeFromSource = propagateChromaSamplingModeFromSource;
    this.samplingMethod = samplingMethod;
    this.useTrellis = useTrellis;
    this.useProgressive = useProgressive;
    this.useOptimizeScan = useOptimizeScan;
    this.useCompatibleDcScanOpt = useCompatibleDcScanOpt;
    this.chromaSamplingModeOverride = chromaSamplingModeOverride;
    this.usePsnrQuantTable = usePsnrQuantTable;
    this.useInterlacing = useInterlacing;
    this.compressionLevel = compressionLevel;
    this.webpMethod = webpMethod;
    this.webpImageHint = webpImageHint;
  }

  public static Configuration makeEmpty() {
    return Builder().build();
  }

  @Override
  public String toString() {
    return "Configuration{"
        + "defaultBackgroundColor="
        + defaultBackgroundColor
        + ", interpretMetadata="
        + interpretMetadata
        + ", samplingMethod="
        + samplingMethod
        + ", chromaSamplingModeOverride="
        + chromaSamplingModeOverride
        + ", propagateChromaSamplingModeFromSource="
        + propagateChromaSamplingModeFromSource
        + ", useTrellis="
        + useTrellis
        + ", useProgressive="
        + useProgressive
        + ", useOptimizeScan="
        + useOptimizeScan
        + ", useCompatibleDcScanOpt="
        + useCompatibleDcScanOpt
        + ", usePsnrQuantTable="
        + usePsnrQuantTable
        + ", useInterlacing="
        + useInterlacing
        + ", compressionLevel="
        + compressionLevel
        + ", webpMethod="
        + webpMethod
        + ", webpImageHint="
        + webpImageHint
        + '}';
  }

  public static Configuration makeForImageContainingGraphics() {
    return new Builder()
        .setUseTrellis(true)
        .setChromaSamplingModeOverride(ImageChromaSamplingMode.S444)
        .setUsePsnrQuantTable(true)
        .build();
  }

  /** @return A new builder to be used for creating {@link Configuration} objects. */
  public static Builder Builder() {
    return new Builder();
  }

  public static class Builder {

    @Nullable private ImageColor mDefaultBackgroundColor;
    @Nullable private Boolean minterpretMetadata;
    @Nullable private SamplingMethod mSamplingMethod;
    @Nullable private Boolean mPropagateChromaSamplingModeFromSource;
    @Nullable private ImageChromaSamplingMode mChromaSamplingModeOverride;
    @Nullable private Boolean mUseTrellis;
    @Nullable private Boolean mUseProgressive;
    @Nullable private Boolean mUseOptimizeScan;
    @Nullable private Boolean mUseCompatibleDcScanOpt;
    @Nullable private Boolean mUsePsnrQuantTable;
    @Nullable private Boolean mUseInterlacing;
    @Nullable private Integer mCompressionLevel;
    @Nullable private Integer mWebpMethod;
    @Nullable private ImageHint mWebpImageHint;

    private Builder() {}

    /** General: Default background color to merge transparent images onto. */
    public Builder setDefaultBackgroundColor(final ImageColor defaultBackgroundColor) {
      mDefaultBackgroundColor = defaultBackgroundColor;
      return this;
    }

    /** General: Whether to read and interpret metadata such as EXIF for JPEG files. */
    public Builder setInterpretMetadata(final Boolean interpretMetadata) {
      minterpretMetadata = interpretMetadata;
      return this;
    }

    /** General: The sampling algorithm to use for the scale operation. */
    public Builder setSamplingMethod(final SamplingMethod samplingMethod) {
      mSamplingMethod = samplingMethod;
      return this;
    }

    /**
     * General: The {@link com.facebook.spectrum.image.ImageChromaSamplingMode} to use when
     * encoding.
     */
    public Builder setChromaSamplingModeOverride(
        final ImageChromaSamplingMode chromaSamplingModeOverride) {
      mChromaSamplingModeOverride = chromaSamplingModeOverride;
      return this;
    }

    /**
     * General: Whether to propagate chroma sampling mode from the source image. If enabled, a
     * source image with S444 will make the output default to S444.
     */
    public Builder setPropagateChromaSamplingModeFromSource(
        final Boolean propagateChromaSamplingModeFromSource) {
      mPropagateChromaSamplingModeFromSource = propagateChromaSamplingModeFromSource;
      return this;
    }

    /** Jpeg: Whether to use trellis optimization when encoding JPEG images. */
    public Builder setUseTrellis(final Boolean useTrellis) {
      mUseTrellis = useTrellis;
      return this;
    }

    /** Jpeg: Whether to save JPEG images in progressive scan encoding. */
    public Builder setUseProgressive(final Boolean useProgressive) {
      mUseProgressive = useProgressive;
      return this;
    }

    /** Jpeg: Whether to use scan optimization when encoding JPEG images. */
    public Builder setUseOptimizeScan(final Boolean useOptimizeScan) {
      mUseOptimizeScan = useOptimizeScan;
      return this;
    }

    /**
     * Jpeg: Whether to use compatible DC scan configuration (keeping all DC components in the first
     * scan) when encoding JPEG images.
     */
    public Builder setUseCompatibleDcScanOpt(final Boolean useCompatibleDcScanOpt) {
      mUseCompatibleDcScanOpt = useCompatibleDcScanOpt;
      return this;
    }

    /**
     * Jpeg: Whether to use an alternative quantization table optimized for PSNR when encoding JPEG
     * images.
     */
    public Builder setUsePsnrQuantTable(final Boolean usePsnrQuantTable) {
      mUsePsnrQuantTable = usePsnrQuantTable;
      return this;
    }

    /** Png: Whether to save PNG images with interlaced encoding. */
    public Builder setUseInterlacing(final Boolean useInterlacing) {
      mUseInterlacing = useInterlacing;
      return this;
    }

    /**
     * Png: The compression level that is used by zlib to determine how much time to spend on trying
     * to compress the image data. 0 is for not using compression at all and 9 is for the best
     * compression.
     */
    public Builder setCompressionLevel(final Integer compressionLevel) {
      Preconditions.checkArgument(compressionLevel >= 0 && compressionLevel <= 9);
      mCompressionLevel = compressionLevel;
      return this;
    }

    /**
     * Webp: Compression quality/speed tradeoff where 0 is the fastest and 6 is the slowest/best
     * compression.
     */
    public Builder setWebpMethod(final Integer webpMethod) {
      mWebpMethod = webpMethod;
      return this;
    }

    /** Webp: Provides an optional {@link ImageHint} to the webp encoder */
    public Builder setWebpImageHint(final ImageHint webpImageHint) {
      mWebpImageHint = webpImageHint;
      return this;
    }

    /**
     * Builds a new {@link Configuration} from this builder.
     *
     * @return a new {@link Configuration} object.
     */
    public Configuration build() {
      return new Configuration(
          mDefaultBackgroundColor,
          minterpretMetadata,
          mSamplingMethod,
          mPropagateChromaSamplingModeFromSource,
          mChromaSamplingModeOverride,
          mUseTrellis,
          mUseProgressive,
          mUseOptimizeScan,
          mUseCompatibleDcScanOpt,
          mUsePsnrQuantTable,
          mUseInterlacing,
          mCompressionLevel,
          mWebpMethod,
          mWebpImageHint);
    }
  }

  /** Optional hint when encoding WebP images */
  @Immutable
  @DoNotStrip
  public enum ImageHint {
    /** Default preset */
    @DoNotStrip
    DEFAULT(0),

    /** General indoor digital picture (e.g. portrait) */
    @DoNotStrip
    PICTURE(1),

    /** General outdoor digital picture (e.g. landscape) */
    @DoNotStrip
    PHOTO(2),

    /** Graphical image */
    @DoNotStrip
    GRAPH(3);

    @DoNotStrip private final int value;

    ImageHint(final int value) {
      this.value = value;
    }

    /** @return matching {@link ImageHint} for the given values. Throws otherwise */
    @DoNotStrip
    static ImageHint from(final int value) {
      for (ImageHint imageHint : ImageHint.values()) {
        if (imageHint.value == value) {
          return imageHint;
        }
      }
      throw new IllegalArgumentException("Unsupported ImageHint");
    }
  }

  /** Sampling algorithms for the exact resize block */
  @Immutable
  @DoNotStrip
  public enum SamplingMethod {
    /** Bicubic filter */
    @DoNotStrip
    Bicubic(1),

    /** Magic kernel implementation with fixed point arithmetic */
    @DoNotStrip
    MagicKernel(2);

    @DoNotStrip private final int value;

    SamplingMethod(final int value) {
      this.value = value;
    }

    /** @return matching {@link SamplingMethod} for the given values. Throws otherwise */
    @DoNotStrip
    static SamplingMethod from(final int value) {
      for (SamplingMethod samplingMethod : SamplingMethod.values()) {
        if (samplingMethod.value == value) {
          return samplingMethod;
        }
      }
      throw new IllegalArgumentException("Unsupported SamplingMethod");
    }
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;

    Configuration that = (Configuration) o;

    if (defaultBackgroundColor != null
        ? !defaultBackgroundColor.equals(that.defaultBackgroundColor)
        : that.defaultBackgroundColor != null) return false;
    if (interpretMetadata != null
        ? !interpretMetadata.equals(that.interpretMetadata)
        : that.interpretMetadata != null) return false;
    if (samplingMethod != that.samplingMethod) return false;
    if (chromaSamplingModeOverride != that.chromaSamplingModeOverride) return false;
    if (propagateChromaSamplingModeFromSource != null
        ? !propagateChromaSamplingModeFromSource.equals(that.propagateChromaSamplingModeFromSource)
        : that.propagateChromaSamplingModeFromSource != null) return false;
    if (useTrellis != null ? !useTrellis.equals(that.useTrellis) : that.useTrellis != null)
      return false;
    if (useProgressive != null
        ? !useProgressive.equals(that.useProgressive)
        : that.useProgressive != null) return false;
    if (useOptimizeScan != null
        ? !useOptimizeScan.equals(that.useOptimizeScan)
        : that.useOptimizeScan != null) return false;
    if (useCompatibleDcScanOpt != null
        ? !useCompatibleDcScanOpt.equals(that.useCompatibleDcScanOpt)
        : that.useCompatibleDcScanOpt != null) return false;
    if (usePsnrQuantTable != null
        ? !usePsnrQuantTable.equals(that.usePsnrQuantTable)
        : that.usePsnrQuantTable != null) return false;
    if (useInterlacing != null
        ? !useInterlacing.equals(that.useInterlacing)
        : that.useInterlacing != null) return false;
    if (compressionLevel != null
        ? !compressionLevel.equals(that.compressionLevel)
        : that.compressionLevel != null) return false;
    if (webpMethod != null ? !webpMethod.equals(that.webpMethod) : that.webpMethod != null)
      return false;
    return webpImageHint == that.webpImageHint;
  }

  @Override
  public int hashCode() {
    return 0;
  }
}
