/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import com.facebook.jni.annotations.DoNotStrip;
import javax.annotation.Nullable;
import javax.annotation.concurrent.Immutable;

/**
 * Represents an image format. The sub-class {@link EncodedImageFormat} indicates that the format is
 * compressed.
 */
@DoNotStrip
@Immutable
public class ImageFormat {

  /** An uncompressed image of continious pixel data */
  public static final ImageFormat BITMAP = new ImageFormat("bitmap");

  @DoNotStrip public final String identifier;

  @DoNotStrip
  public ImageFormat(final String identifier) {
    this.identifier = identifier;
  }

  @DoNotStrip
  public boolean isEncoded() {
    return false;
  }

  @Override
  @DoNotStrip
  public boolean equals(@Nullable final Object o) {
    if (o == null) {
      return false;
    } else if (this == o) {
      return true;
    } else if (!(o instanceof ImageFormat)) {
      return false;
    } else {
      return identifier.equals(((ImageFormat) o).identifier);
    }
  }

  @Override
  @DoNotStrip
  public String toString() {
    return identifier;
  }

  @Override
  @DoNotStrip
  public int hashCode() {
    return identifier.hashCode();
  }
}
