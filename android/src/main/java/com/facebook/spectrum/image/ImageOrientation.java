/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import com.facebook.jni.annotations.DoNotStrip;
import javax.annotation.concurrent.Immutable;

/**
 * The orientation describes the transformation that needs to be applied to the underlying image in
 * order to reach its intended representation.
 */
@DoNotStrip
@Immutable
public enum ImageOrientation {
  @DoNotStrip
  UP((short) 1),

  @DoNotStrip
  UP_MIRRORED((short) 2),

  @DoNotStrip
  BOTTOM((short) 3),

  @DoNotStrip
  BOTTOM_MIRRORED((short) 4),

  @DoNotStrip
  LEFT_MIRRORED((short) 5),

  @DoNotStrip
  RIGHT((short) 6),

  @DoNotStrip
  RIGHT_MIRRORED((short) 7),

  @DoNotStrip
  LEFT((short) 8);

  @DoNotStrip public final short value;

  @DoNotStrip
  ImageOrientation(final short value) {
    this.value = value;
  }

  /**
   * @return matching {@link ImageOrientation} for the given value. Returns <code>null</code> if
   *     none matches.
   */
  @DoNotStrip
  static ImageOrientation from(final short value) {
    for (ImageOrientation orientation : values()) {
      if (orientation.value == value) {
        return orientation;
      }
    }
    throw new IllegalArgumentException("Unsupported ImageOrientation");
  }
}
