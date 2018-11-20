/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.utils.Preconditions;
import javax.annotation.concurrent.Immutable;

/** Represents a rectangular area defined by its width and height. */
@DoNotStrip
@Immutable
public class ImageSize {

  /**
   * Setting to 2**16 as this is the smallest common denominator for all common image libraries that
   * we are interested in.
   */
  @DoNotStrip public static final int MAX_IMAGE_SIDE_DIMENSION = 65536;

  /** The size's width. */
  @DoNotStrip public final int width;

  /** The size's height. */
  @DoNotStrip public final int height;

  /**
   * Creates a new {@link ImageSize}
   *
   * @param width Must be within [0, {@link #MAX_IMAGE_SIDE_DIMENSION}]
   * @param height Must be within [0, {@link #MAX_IMAGE_SIDE_DIMENSION}]
   */
  @DoNotStrip
  public ImageSize(final int width, final int height) {
    Preconditions.checkArgument(width >= 0 && width <= MAX_IMAGE_SIDE_DIMENSION);
    Preconditions.checkArgument(height >= 0 && height <= MAX_IMAGE_SIDE_DIMENSION);
    this.width = width;
    this.height = height;
  }

  @Override
  public boolean equals(final Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;

    final ImageSize that = (ImageSize) o;
    return width == that.width && height == that.height;
  }

  @Override
  public int hashCode() {
    int result = width;
    result = 31 * result + height;
    return result;
  }

  @Override
  public String toString() {
    return "ImageSize{" + "width=" + width + ", height=" + height + '}';
  }
}
