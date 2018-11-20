/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.requirements;

import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.image.EncodedImageFormat;
import javax.annotation.concurrent.Immutable;

/**
 * Defines how an image should be encoded. This includes the {@link EncodedImageFormat}, the {@link
 * EncodeRequirement.Mode}, and the quality level.
 */
@DoNotStrip
@Immutable
public class EncodeRequirement {

  public static final int QUALITY_UNSET = 0;
  public static final int QUALITY_MIN = 1;
  public static final int QUALITY_MAX = 100;

  @DoNotStrip
  public enum Mode {
    @DoNotStrip
    LOSSLESS(0),

    @DoNotStrip
    LOSSY(1),

    @DoNotStrip
    ANY(2);

    @DoNotStrip private final int value;

    Mode(final int value) {
      this.value = value;
    }

    /** @return matching {@link Mode} for the given values. Throws otherwise */
    @DoNotStrip
    static Mode from(final int value) {
      for (Mode mode : Mode.values()) {
        if (mode.value == value) {
          return mode;
        }
      }
      throw new IllegalArgumentException("Unsupported EncodeRequirement.Mode");
    }
  }

  /** The format of the target encoded image. */
  @DoNotStrip public final EncodedImageFormat format;

  /**
   * The quality used to encode the image. Must be within the range of [1, 100]. 0 is considered
   * unset and a default value for the format is used.
   */
  @DoNotStrip public final int quality;

  /** The compression mode. */
  @DoNotStrip public final Mode mode;

  /**
   * Creates a new {@link EncodeRequirement} for the given {@link EncodedImageFormat}. The result is
   * either {@link Mode#LOSSLESS} or {@link Mode#LOSSY}. A default quality parameter will be used.
   */
  @DoNotStrip
  public EncodeRequirement(final EncodedImageFormat format) {
    this(format, Mode.ANY);
  }

  /**
   * Creates a new {@link EncodeRequirement} for the given {@link EncodedImageFormat} and {@link
   * Mode}. A default quality parameter will be used.
   */
  @DoNotStrip
  public EncodeRequirement(final EncodedImageFormat format, final Mode mode) {
    this(format, QUALITY_UNSET, mode);
  }

  /**
   * Creates a new {@link EncodeRequirement} for the given {@link EncodedImageFormat} and quality.
   * The result is either {@link Mode#LOSSLESS} or {@link Mode#LOSSY}.
   */
  @DoNotStrip
  public EncodeRequirement(final EncodedImageFormat format, final int quality) {
    this(format, quality, Mode.ANY);
  }

  /**
   * Creates a new {@link EncodeRequirement} for the given {@link EncodedImageFormat}, {@link Mode},
   * and quality.
   */
  @DoNotStrip
  public EncodeRequirement(final EncodedImageFormat format, final int quality, final Mode mode) {
    this.format = format;
    this.quality = quality;
    this.mode = mode;
  }

  @Override
  public boolean equals(Object o) {
    if (o == null) {
      return false;
    } else if (this == o) {
      return true;
    } else if (!(o instanceof EncodeRequirement)) {
      return false;
    } else {
      EncodeRequirement other = (EncodeRequirement) o;
      return format.equals(other.format) && quality == other.quality && mode == other.mode;
    }
  }

  @Override
  public String toString() {
    return "EncodeRequirement{"
        + "format="
        + format.identifier
        + ", quality="
        + quality
        + ", mode="
        + mode
        + '}';
  }
}
