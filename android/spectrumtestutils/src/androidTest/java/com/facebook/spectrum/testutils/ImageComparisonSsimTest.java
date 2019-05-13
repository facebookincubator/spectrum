/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import static org.fest.assertions.api.Assertions.assertThat;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import com.facebook.spectrum.testutils.ImageComparison.ImageComparisonMethod;
import java.io.IOException;
import org.fest.assertions.data.Offset;
import org.junit.Test;

public class ImageComparisonSsimTest {

  private static final Offset<Float> EPS = Offset.offset(0.03f);

  @Test
  public void testCompare_whenIdentical_thenScoreEqual1() throws IOException {
    final Bitmap a = TestData.getBitmap(TestData.JPEG.PATH_85x128_Q85);
    final Bitmap b = TestData.getBitmap(TestData.JPEG.PATH_85x128_Q85);

    final float score = ImageComparison.compare(a, b, ImageComparisonMethod.SSIM);
    assertThat(score).isEqualTo(1.00f, EPS);
  }

  @Test
  public void testCompare_whenCompletelyDifferent_thenScoreEqual0() {
    final Bitmap a = Bitmap.createBitmap(100, 100, Bitmap.Config.ARGB_8888);
    final Bitmap b = Bitmap.createBitmap(100, 100, Bitmap.Config.ARGB_8888);

    final Canvas canvasA = new Canvas(a);
    canvasA.drawColor(Color.WHITE);

    final Canvas canvasB = new Canvas(b);
    canvasB.drawColor(Color.BLACK);

    final float score = ImageComparison.compare(a, b, ImageComparisonMethod.SSIM);
    assertThat(score).isEqualTo(0.00f, EPS);
  }

  @Test
  public void testCompare_whenDistortedByOneRect_thenScoreSmallerThan1() throws IOException {
    final Bitmap a = TestData.getBitmap(TestData.JPEG.PATH_85x128_Q85);
    final Bitmap b = TestData.getBitmap(TestData.JPEG.PATH_85x128_Q85);

    final Bitmap mutableB = b.copy(Bitmap.Config.ARGB_8888, true);
    final Canvas canvasB = new Canvas(mutableB);
    final Paint paint = new Paint();
    paint.setColor(Color.RED);
    canvasB.drawRect(10f, 10f, 20f, 20f, paint);

    final float score = ImageComparison.compare(a, mutableB, ImageComparisonMethod.SSIM);
    assertThat(score).isLessThan(1f);
    assertThat(score).isEqualTo(0.97f, EPS);
  }

  @Test
  public void testCompare_whenDistortedByTwoRects_thenScoreSmallerThan1_andSmallerThanWithOneRect()
      throws IOException {
    final Bitmap a = TestData.getBitmap(TestData.JPEG.PATH_85x128_Q85);
    final Bitmap b = TestData.getBitmap(TestData.JPEG.PATH_85x128_Q85);

    final Bitmap mutableB = b.copy(Bitmap.Config.ARGB_8888, true);
    final Canvas canvasB = new Canvas(mutableB);
    final Paint paint = new Paint();
    paint.setColor(Color.RED);
    canvasB.drawRect(10f, 10f, 20f, 20f, paint);
    paint.setColor(Color.GREEN);
    canvasB.drawRect(30f, 30f, 40f, 40f, paint);

    final float score = ImageComparison.compare(a, mutableB, ImageComparisonMethod.SSIM);
    assertThat(score).isLessThan(0.97f);
    assertThat(score).isEqualTo(0.95f, EPS);
  }

  @Test
  public void testCompare_whenDistortedByWorseQuality_thenScoreSmallerThan1() throws IOException {
    final Bitmap a = TestData.getBitmap(TestData.JPEG.PATH_85x128_Q85);
    final Bitmap b = TestData.getBitmap(TestData.JPEG.PATH_85x128_Q15);

    final float score = ImageComparison.compare(a, b, ImageComparisonMethod.SSIM);
    assertThat(score).isEqualTo(0.81f, EPS);
  }
}
