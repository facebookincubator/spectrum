/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import static org.fest.assertions.api.Assertions.assertThat;

import android.graphics.Bitmap;
import java.io.IOException;
import org.junit.Test;

public class TestDataTest {

  @Test
  public void testOpenTestData_whenExists_thenSucceedsAndCanBeDecoded() throws IOException {
    final String[] testAssets =
        new String[] {TestData.SAMPLE_85x128_q15, TestData.SAMPLE_85x128_q85};
    for (String testAsset : testAssets) {
      final Bitmap b = TestData.getBitmap(testAsset);
      assertThat(b.getWidth()).isEqualTo(85);
      assertThat(b.getHeight()).isEqualTo(128);
      assertThat(b.getConfig()).isEqualTo(Bitmap.Config.ARGB_8888);
    }
  }
}
