/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import android.graphics.Bitmap;
import java.util.Locale;

/**
 * Simple implementations of image comparison algorithms for instrumentation test purposes. As these
 * are naive and unoptimized implementations do not use them in production code.
 */
public final class ImageComparison {

  public enum ImageComparisonMethod {
    SSIM
  }

  public static float compare(Bitmap a, Bitmap b, ImageComparisonMethod comparisonMethod) {
    if (a.getHeight() != b.getHeight()
        || a.getWidth() != b.getWidth()
        || a.getConfig() != b.getConfig()) {
      throw new IllegalArgumentException(
          String.format(
              (Locale) null,
              "Bitmaps must match in size and configuration a=(%d %d) does not match b=(%d %d)",
              a.getWidth(),
              a.getHeight(),
              b.getWidth(),
              b.getHeight()));
    }

    switch (comparisonMethod) {
      case SSIM:
        return ImageComparisonSsim.compareSsim(a, b);
      default:
        throw new IllegalArgumentException("Not yet implemented: " + comparisonMethod);
    }
  }
}
