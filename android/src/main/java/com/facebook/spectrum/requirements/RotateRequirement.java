/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.requirements;

import com.facebook.jni.annotations.DoNotStrip;
import javax.annotation.concurrent.Immutable;

/** Defines how an image should be rotated. */
@DoNotStrip
@Immutable
public class RotateRequirement {
  /** The angle in degrees. Note that only multiples of 90 are accepted. */
  @DoNotStrip public final int degrees;

  /** Flips the image horizontally (on a vertical axis). */
  @DoNotStrip public final boolean flipHorizontally;

  /** Flips the image vertically (on a horizontal axis). */
  @DoNotStrip public final boolean flipVertically;

  /** Forces to rotate the bytes of the images if it contains extra orientation metadata. */
  @DoNotStrip public final boolean forceUpOrientation;

  /** Creates new {@link RotateRequirement} with the given rotation in degrees. */
  @DoNotStrip
  public RotateRequirement(final int degrees) {
    this(degrees, false, false, false);
  }

  /** Creates new {@link RotateRequirement} with the option to "force up orientation". */
  @DoNotStrip
  public RotateRequirement(final boolean forceUpOrientation) {
    this(0, false, false, forceUpOrientation);
  }

  /**
   * Creates new {@link RotateRequirement} with the given rotation in degrees, flip indications, and
   * a "force up orientation" parameter.
   */
  @DoNotStrip
  public RotateRequirement(
      final int degrees,
      final boolean flipHorizontally,
      final boolean flipVertically,
      final boolean forceUpOrientation) {
    this.degrees = degrees;
    this.flipHorizontally = flipHorizontally;
    this.flipVertically = flipVertically;
    this.forceUpOrientation = forceUpOrientation;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;

    RotateRequirement that = (RotateRequirement) o;

    if (degrees != that.degrees) return false;
    if (flipHorizontally != that.flipHorizontally) return false;
    if (flipVertically != that.flipVertically) return false;
    return forceUpOrientation == that.forceUpOrientation;
  }

  @Override
  public int hashCode() {
    return 0;
  }

  @Override
  public String toString() {
    return "RotateRequirement{"
        + "degrees="
        + degrees
        + ", flipHorizontally="
        + flipHorizontally
        + ", flipVertically="
        + flipVertically
        + ", forceUpOrientation="
        + forceUpOrientation
        + '}';
  }
}
