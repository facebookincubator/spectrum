/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import android.graphics.Bitmap;
import com.facebook.spectrum.testutils.ImageComparison.ImageComparisonMethod;
import org.junit.Test;

public class ImageComparisonTest {

  @Test(expected = IllegalArgumentException.class)
  public void testCompare_whenBitmapsDoNotMatchInHeight_thenThrow() {
    Bitmap a = Bitmap.createBitmap(200, 100, Bitmap.Config.ARGB_8888);
    Bitmap b = Bitmap.createBitmap(200, 101, Bitmap.Config.ARGB_8888);
    ImageComparison.compare(a, b, ImageComparisonMethod.SSIM);
  }

  @Test(expected = IllegalArgumentException.class)
  public void testCompare_whenBitmapsDoNotMatchInWidth_thenThrow() {
    Bitmap a = Bitmap.createBitmap(200, 100, Bitmap.Config.ARGB_8888);
    Bitmap b = Bitmap.createBitmap(201, 100, Bitmap.Config.ARGB_8888);
    ImageComparison.compare(a, b, ImageComparisonMethod.SSIM);
  }

  @Test(expected = IllegalArgumentException.class)
  public void testCompare_whenBitmapsDoNotMatchInConfig_thenThrow() {
    Bitmap a = Bitmap.createBitmap(200, 100, Bitmap.Config.ARGB_8888);
    Bitmap b = Bitmap.createBitmap(200, 100, Bitmap.Config.RGB_565);
    ImageComparison.compare(a, b, ImageComparisonMethod.SSIM);
  }
}
