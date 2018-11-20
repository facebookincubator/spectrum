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
 * Metadata represents information that is stored in images beside their pixel values. The most
 * common example are EXIF metadata and ICC profiles.
 *
 * <p>This is not yet implemented in the Android wrapper.
 */
@DoNotStrip
@Immutable
public class ImageMetadata {

  @Override
  public boolean equals(final Object obj) {
    return obj instanceof ImageMetadata;
  }

  @Override
  public int hashCode() {
    return 0;
  }

  public static ImageMetadata empty() {
    return new ImageMetadata();
  }

  @Override
  public String toString() {
    return "ImageMetadata{}";
  }
}
