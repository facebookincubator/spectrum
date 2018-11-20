/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.options;

import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.requirements.CropRequirement;
import com.facebook.spectrum.requirements.ResizeRequirement;
import com.facebook.spectrum.requirements.RotateRequirement;
import javax.annotation.Nullable;
import javax.annotation.concurrent.Immutable;

/** Image transformations that can be applied to an operation. */
@DoNotStrip
@Immutable
public final class Transformations {

  /** The {@link ResizeRequirement} to apply. */
  @DoNotStrip @Nullable public final ResizeRequirement resizeRequirement;

  /** The {@link CropRequirement} to apply. */
  @DoNotStrip @Nullable public final CropRequirement cropRequirement;

  /** The rotate requirement to apply. */
  @DoNotStrip @Nullable public final RotateRequirement rotateRequirement;

  /* package */ Transformations(
      @Nullable final ResizeRequirement resizeRequirement,
      @Nullable final CropRequirement cropRequirement,
      @Nullable final RotateRequirement rotateRequirement) {
    this.resizeRequirement = resizeRequirement;
    this.cropRequirement = cropRequirement;
    this.rotateRequirement = rotateRequirement;
  }

  @Override
  public String toString() {
    return "Transformations{"
        + "resizeRequirement="
        + resizeRequirement
        + ", cropRequirement="
        + cropRequirement
        + ", rotateRequirement="
        + rotateRequirement
        + '}';
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;

    Transformations that = (Transformations) o;

    if (resizeRequirement != null
        ? !resizeRequirement.equals(that.resizeRequirement)
        : that.resizeRequirement != null) return false;
    if (cropRequirement != null
        ? !cropRequirement.equals(that.cropRequirement)
        : that.cropRequirement != null) return false;
    return rotateRequirement != null
        ? rotateRequirement.equals(that.rotateRequirement)
        : that.rotateRequirement == null;
  }

  @Override
  public int hashCode() {
    return 0;
  }
}
