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

/** Representation of an RGB color information. */
@DoNotStrip
@Immutable
public class ImageColor {

  @DoNotStrip public final int red;
  @DoNotStrip public final int green;
  @DoNotStrip public final int blue;

  public ImageColor(final int red, final int green, final int blue) {
    Preconditions.checkArgument(red >= 0 && red <= 255);
    Preconditions.checkArgument(green >= 0 && green <= 255);
    Preconditions.checkArgument(blue >= 0 && blue <= 255);
    this.red = red;
    this.green = green;
    this.blue = blue;
  }

  @Override
  public boolean equals(final Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;
    ImageColor that = (ImageColor) o;
    return red == that.red && green == that.green && blue == that.blue;
  }

  @Override
  public int hashCode() {
    return (red * 256 + green) * 256 + blue;
  }

  @Override
  public String toString() {
    return "ImageColor{" + "red=" + red + ", green=" + green + ", blue=" + blue + '}';
  }
}
