/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import androidx.test.InstrumentationRegistry;
import java.io.IOException;
import java.io.InputStream;

public class TestData {

  static final String SAMPLE_85x128_q15 = "sample_85x128_q15.jpg";
  static final String SAMPLE_85x128_q85 = "sample_85x128_q85.jpg";

  static InputStream getInputStream(final String path) throws IOException {
    return getContext().getResources().getAssets().open(path);
  }

  static Bitmap getBitmap(final String path) throws IOException {
    InputStream inputStream = null;
    //noinspection TryFinallyCanBeTryWithResources
    try {
      inputStream = getInputStream(path);
      return BitmapFactory.decodeStream(inputStream);
    } finally {
      if (inputStream != null) {
        inputStream.close();
      }
    }
  }

  private static Context getContext() {
    return InstrumentationRegistry.getContext();
  }
}
