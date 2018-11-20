/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.options;

import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.Configuration;
import com.facebook.spectrum.image.ImageMetadata;
import com.facebook.spectrum.image.ImagePixelSpecification;
import com.facebook.spectrum.image.ImageSize;
import com.facebook.spectrum.requirements.CropRequirement;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.requirements.ResizeRequirement;
import com.facebook.spectrum.requirements.RotateRequirement;
import com.facebook.spectrum.utils.Preconditions;
import javax.annotation.Nullable;
import javax.annotation.concurrent.Immutable;

/**
 * The Options object encapsulates all parameters for an operation. In most cases subclasses ({@link
 * DecodeOptions}, {@link TranscodeOptions}, {@link EncodeOptions}, {@link TransformOptions}) should
 * be used instead of this type directly.
 */
@DoNotStrip
@Immutable
public class Options {

  /**
   * The {@link EncodeRequirement} to apply. Dictates the final image format. If none, bitmap is
   * deduced.
   */
  @DoNotStrip @Nullable public final EncodeRequirement encodeRequirement;

  /** Contains optional {@link Transformations} such as resize, cropping and rotation. */
  @DoNotStrip public final Transformations transformations;

  /** Optional metadata to consider besides the ones provided by the encoded image. */
  @DoNotStrip @Nullable public final ImageMetadata metadata;

  /** Options in the configuration object will override default and app-wide settings. */
  @DoNotStrip @Nullable public final Configuration configuration;
  /**
   * If set, will dictate the pixel specification images should be converted to before being passed
   * to the compressor. An exception will be thrown if the pixel specification doesn't fit the image
   * format.
   */
  @DoNotStrip @Nullable public final ImagePixelSpecification outputPixelSpecification;

  protected Options(final Builder builder) {
    encodeRequirement = builder.encodeRequirement;
    transformations =
        new Transformations(
            builder.resizeRequirement, builder.cropRequirement, builder.rotateRequirement);
    metadata = builder.metadata;
    configuration = builder.configuration;
    outputPixelSpecification = builder.outputPixelSpecification;
  }

  /** For object construction in JNI */
  @DoNotStrip
  private Options(
      @Nullable EncodeRequirement encodeRequirement,
      Transformations transformations,
      @Nullable ImageMetadata metadata,
      @Nullable Configuration configuration,
      @Nullable ImagePixelSpecification outputPixelSpecification) {
    this.encodeRequirement = encodeRequirement;
    this.transformations = transformations;
    this.metadata = metadata;
    this.configuration = configuration;
    this.outputPixelSpecification = outputPixelSpecification;
  }

  public String toString(final String className) {
    return className
        + "{"
        + "encodeRequirement="
        + encodeRequirement
        + ", transformations="
        + transformations
        + ", metadata="
        + metadata
        + ", configuration="
        + configuration
        + ", outputPixelSpecification="
        + outputPixelSpecification
        + '}';
  }

  @Override
  public String toString() {
    return toString("Options");
  }

  @SuppressWarnings("unchecked")
  public abstract static class Builder<T extends Builder<T>> {

    @Nullable protected EncodeRequirement encodeRequirement = null;
    @Nullable protected ResizeRequirement resizeRequirement = null;
    @Nullable protected RotateRequirement rotateRequirement = null;
    @Nullable protected CropRequirement cropRequirement = null;
    @Nullable protected ImageMetadata metadata = null;
    @Nullable protected Configuration configuration = null;
    @Nullable protected ImagePixelSpecification outputPixelSpecification = null;

    protected Builder() {}

    /** Options in the configuration object will override default and app-wide settings. */
    public T configuration(final Configuration configuration) {
      this.configuration = Preconditions.checkNotNull(configuration);
      return (T) this;
    }

    /** The resize requirement to apply. */
    public T resize(final ResizeRequirement resizeRequirement) {
      this.resizeRequirement = Preconditions.checkNotNull(resizeRequirement);
      return (T) this;
    }

    /**
     * The resize requirement to apply. See {@link
     * ResizeRequirement#ResizeRequirement(ResizeRequirement.Mode, ImageSize)}
     */
    public T resize(final ResizeRequirement.Mode mode, final ImageSize targetSize) {
      return resize(new ResizeRequirement(mode, targetSize));
    }

    /**
     * The resize requirement to apply. See {@link
     * ResizeRequirement#ResizeRequirement(ResizeRequirement.Mode, int)}
     */
    public T resize(final ResizeRequirement.Mode mode, final int sideLength) {
      return resize(new ResizeRequirement(mode, sideLength));
    }

    /** The crop requirement to apply. */
    public T crop(final CropRequirement cropRequirement) {
      this.cropRequirement = Preconditions.checkNotNull(cropRequirement);
      return (T) this;
    }

    /**
     * The crop requirement to apply. See {@link CropRequirement#makeAbsoluteToOrigin(int, int, int,
     * int, boolean)}
     */
    public T cropAbsoluteToOrigin(
        final int left,
        final int top,
        final int right,
        final int bottom,
        final boolean mustBeExact) {
      return crop(CropRequirement.makeAbsoluteToOrigin(left, top, right, bottom, mustBeExact));
    }

    /**
     * The crop requirement to apply. See {@link CropRequirement#makeRelativeToOrigin(float, float,
     * float, float, boolean)}
     */
    public T cropRelativeToOrigin(
        final float left,
        final float top,
        final float right,
        final float bottom,
        final boolean mustBeExact) {
      return crop(CropRequirement.makeRelativeToOrigin(left, top, right, bottom, mustBeExact));
    }

    /** The rotate requirement to apply. */
    public T rotate(final RotateRequirement rotateRequirement) {
      this.rotateRequirement = rotateRequirement;
      return (T) this;
    }

    /**
     * The rotate requirement to apply. See {@link RotateRequirement#RotateRequirement(int, boolean,
     * boolean, boolean)}
     */
    public T rotate(
        final int degrees,
        final boolean flipHorizontally,
        final boolean flipVertically,
        final boolean forceUpOrientation) {
      return rotate(
          new RotateRequirement(degrees, flipHorizontally, flipVertically, forceUpOrientation));
    }

    /**
     * The rotates the image by the given amount of degrees. See {@link
     * RotateRequirement#RotateRequirement(int)}
     */
    public T rotate(final int degrees) {
      return rotate(new RotateRequirement(degrees));
    }

    public abstract Options build();
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (!(o instanceof Options)) return false;

    final Options options = (Options) o;

    if (encodeRequirement != null
        ? !encodeRequirement.equals(options.encodeRequirement)
        : options.encodeRequirement != null) return false;
    if (transformations != null
        ? !transformations.equals(options.transformations)
        : options.transformations != null) return false;
    if (metadata != null ? !metadata.equals(options.metadata) : options.metadata != null)
      return false;
    if (configuration != null
        ? !configuration.equals(options.configuration)
        : options.configuration != null) return false;
    return outputPixelSpecification == options.outputPixelSpecification;
  }

  @Override
  public int hashCode() {
    return 0;
  }
}
