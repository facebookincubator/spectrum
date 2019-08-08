/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import static org.fest.assertions.api.Assertions.assertThat;

import android.graphics.Bitmap;
import java.io.InputStream;
import java.lang.reflect.Field;
import org.junit.Test;

public class TestDataTest {

  @Test
  public void testOpenTestData_whenReadingAllJpegFiles_thenAllDecode() throws Exception {
    openAndAssertAllFields(TestData.JPEG.class, true);
  }

  @Test
  public void testOpenTestData_whenReadingAllPngFiles_thenAllDecode() throws Exception {
    openAndAssertAllFields(TestData.PNG.class, true);
  }

  @Test
  public void testOpenTestData_whenReadingAllWebpFiles_thenAllDecode() throws Exception {
    openAndAssertAllFields(TestData.WEBP.class, true);
  }

  @Test
  public void testOpenTestData_whenReadingAllIvfAv1Files_thenAllAreNonEmpty() throws Exception {
    openAndAssertAllFields(TestData.IVFAV1.class, false);
  }

  @Test
  public void testOpenTestData_whenReadingAllGifFiles_thenAllAreNonEmpty() throws Exception {
    openAndAssertAllFields(TestData.GIF.class, true);
  }

  private void openAndAssertAllFields(final Class clazz, final boolean assertSuccessfulDecode)
      throws Exception {
    final Field[] allFields = clazz.getFields();
    for (final Field field : allFields) {
      final String path = (String) field.get(null);

      final InputStream inputStream = TestData.getInputStream(path);
      assertThat(inputStream.available()).isGreaterThan(0);
      inputStream.close();

      if (assertSuccessfulDecode) {
        final Bitmap b = TestData.getBitmap(path);
        assertThat(b.getWidth()).isGreaterThan(0);
        assertThat(b.getHeight()).isGreaterThan(0);
        assertThat(b.getConfig()).isEqualTo(Bitmap.Config.ARGB_8888);
      }
    }
  }
}
