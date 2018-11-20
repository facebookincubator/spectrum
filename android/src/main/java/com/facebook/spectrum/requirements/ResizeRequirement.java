/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.requirements;

import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.image.ImageSize;
import com.facebook.spectrum.utils.Preconditions;
import javax.annotation.concurrent.Immutable;

/**
 * Defines how an image should be resized.
 *
 * <p>A resize requirement will never affect the aspect ratio of an image and will never upscale an
 * image.
 */
@DoNotStrip
@Immutable
public class ResizeRequirement {
  /** The mode to resize the image with. */
  @DoNotStrip public final Mode mode;

  /** The target size to resize the image to. */
  @DoNotStrip public final ImageSize targetSize;

  /**
   * Creates new resize requirement.
   *
   * @param mode Defines the strictness of the resize requirement in accepting values other than the
   *     specified target size.
   * @param targetSize The target size to resize the image to.
   */
  public ResizeRequirement(final Mode mode, final ImageSize targetSize) {
    this.mode = Preconditions.checkNotNull(mode);
    this.targetSize = Preconditions.checkNotNull(targetSize);
    Preconditions.checkArgument(targetSize.width > 0);
    Preconditions.checkArgument(targetSize.height > 0);
  }

  /**
   * Creates new resize requirement for a square target size.
   *
   * @param mode Defines the strictness of the resize requirement in accepting values other than the
   *     specified target size.
   * @param sideLength The target size length
   */
  public ResizeRequirement(final Mode mode, final int sideLength) {
    this(mode, new ImageSize(sideLength, sideLength));
  }

  @Override
  public String toString() {
    return "ResizeRequirement{" + "mode=" + mode + ", targetSize=" + targetSize + '}';
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;

    ResizeRequirement that = (ResizeRequirement) o;

    if (mode != that.mode) return false;
    return targetSize != null ? targetSize.equals(that.targetSize) : that.targetSize == null;
  }

  @Override
  public int hashCode() {
    return 0;
  }

  /**
   * Defines the strictness of the resize requirement in accepting values other than the specified
   * target size.
   */
  @DoNotStrip
  public enum Mode {
    /** When resizing, the output image will have the exact target size. */
    @DoNotStrip
    EXACT(0),

    /**
     * When resizing, the output image might be slightly smaller than the target size. This can
     * allow for more efficient operations.
     */
    @DoNotStrip
    EXACT_OR_SMALLER(1),

    /**
     * When resizing, the output image might be slightly larger than the target size. This can allow
     * for more efficient operations.
     */
    @DoNotStrip
    EXACT_OR_LARGER(2);

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
      throw new IllegalArgumentException("Unsupported Mode");
    }
  }
}
