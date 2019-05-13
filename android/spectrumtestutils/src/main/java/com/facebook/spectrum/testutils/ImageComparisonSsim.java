/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import android.graphics.Bitmap;
import com.facebook.spectrum.utils.Preconditions;

/** Simple (approximate) implementation of the SSIM image similarity metric. */
/* package */ final class ImageComparisonSsim {

  private static final float SSIM_K1 = 0.01f;
  private static final float SSIM_K2 = 0.03f;
  private static final float SSIM_MAX_VALUE = 256f;

  private static final float SSIM_C1 = (SSIM_K1 * SSIM_MAX_VALUE) * (SSIM_K1 * SSIM_MAX_VALUE);
  private static final float SSIM_C2 = (SSIM_K2 * SSIM_MAX_VALUE) * (SSIM_K2 * SSIM_MAX_VALUE);

  private static final int SSIM_WINDOW_SIZE = 8;
  private static final int SSIM_WINDOW_STEP = 4;

  private static final int MASK_RIGHT_MOST_CHANNEL = 0x000000FF;

  /**
   * Complexity: O(channels * w/stepSize * h/stepSize * windowsSize^2)
   *
   * <p>Given that channels=3, stepSize=4 and windowSize=8 this results in O(bitmapSize) with a
   * large constant factor. This is usually too slow for large images in production scenarios, but
   * it works fine for tests.
   *
   * <p>Memory complexity is O(windowSize^2)
   */
  /* package */ static float compareSsim(final Bitmap a, final Bitmap b) {
    Preconditions.checkArgument(a.getWidth() == b.getWidth());
    Preconditions.checkArgument(a.getHeight() == b.getHeight());

    final int w = a.getWidth();
    final int h = a.getHeight();
    Preconditions.checkArgument(w >= SSIM_WINDOW_SIZE);
    Preconditions.checkArgument(h >= SSIM_WINDOW_SIZE);

    final int l = SSIM_WINDOW_SIZE * SSIM_WINDOW_SIZE;
    int[] pixelsA = new int[l];
    int[] pixelsB = new int[l];
    int[] channelA = new int[l];
    int[] channelB = new int[l];

    float ssimSum = 0.0f;
    int ssimRuns = 0;

    for (int x = 0; x < w - SSIM_WINDOW_SIZE; x += SSIM_WINDOW_STEP) {
      for (int y = 0; y < h - SSIM_WINDOW_SIZE; y += SSIM_WINDOW_STEP) {
        a.getPixels(pixelsA, 0, SSIM_WINDOW_SIZE, x, y, SSIM_WINDOW_SIZE, SSIM_WINDOW_SIZE);
        b.getPixels(pixelsB, 0, SSIM_WINDOW_SIZE, x, y, SSIM_WINDOW_SIZE, SSIM_WINDOW_SIZE);

        for (int channel = 0; channel < 2; channel++) {
          final int shift = channel * 8;

          for (int i = 0; i < l; i++) {
            channelA[i] = (pixelsA[i] >> shift) & MASK_RIGHT_MOST_CHANNEL;
            channelB[i] = (pixelsB[i] >> shift) & MASK_RIGHT_MOST_CHANNEL;
          }

          final float ssim = ssimForWindow(l, channelA, channelB);
          ssimSum += ssim;
          ssimRuns += 1;
        }
      }
    }

    return ssimSum / (float) ssimRuns;
  }

  /**
   * Complexity: O(l)
   *
   * <p>l usually is ~windowSize^2
   */
  private static float ssimForWindow(final int l, final int[] a, final int[] b) {
    Preconditions.checkArgument(l == a.length);
    Preconditions.checkArgument(l == b.length);

    // calculate avg
    float avgA = 0.0f, avgB = 0.0f;
    for (int i = 0; i < l; i++) {
      avgA += a[i];
      avgB += b[i];
    }
    avgA /= (float) l;
    avgB /= (float) l;

    // calculate variance
    float varA = 0.0f, varB = 0.0f;
    for (int i = 0; i < l; i++) {
      final float ta = a[i] - avgA;
      final float tb = b[i] - avgB;
      varA += ta * ta;
      varB += tb * tb;
    }
    varA /= (float) l;
    varB /= (float) l;

    // calculate co-variance
    float covAB = 0.0f;
    for (int i = 0; i < l; i++) {
      covAB += (a[i] - avgA) * (b[i] - avgB);
    }
    covAB /= (float) l;

    // combine in SSIM formula
    final float t1 = 2 * avgA * avgB + SSIM_C1;
    final float t2 = 2 * covAB + SSIM_C2;
    final float t3 = avgA * avgA + avgB * avgB + SSIM_C1;
    final float t4 = varA + varB + SSIM_C2;
    return (t1 * t2) / (t3 * t4);
  }
}
