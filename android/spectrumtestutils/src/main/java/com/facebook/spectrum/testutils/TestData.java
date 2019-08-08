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
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class TestData {

  public interface JPEG {
    String PATH_85x128_Q15 = "sample_85x128_q15.jpg";
    String PATH_85x128_Q85 = "sample_85x128_q85.jpg";

    String PATH_128x85_Q75_BASELINE = "128x85_q75_baseline.jpg";
    String PATH_128x85_Q75_GRAYSCALE = "128x85_q75_grayscale.jpg";
    String PATH_128x85_Q75_PROGRESSIVE = "128x85_q75_progressive.jpg";
    String PATH_800x530_Q75_BASELINE = "800x530_q75_baseline.jpg";
    String PATH_800x530_Q75_GRAYSCALE = "800x530_q75_grayscale.jpg";
    String PATH_800x530_Q75_PROGRESSIVE = "800x530_q75_progressive.jpg";

    String PATH_128x85_Q75_PROGRESSIVE_R90 = "128x85_q75_progressive_r90.jpg";

    String PATH_CROPPED_200x100_100_50_Q95 = "c200x100_o100_50_q95.jpg";
    String PATH_CROPPED_RELATIVE_50Px60P_CENTER_Q95 = "c50px60p_center_q95.jpg";

    // jpegtran -trim -crop 128x64+32+16 800x530_q75_baseline.jpg >
    // jpegtran_800x530_c128x64ox32oy16.jpg
    String PATH_JPEGTRAN_800x530_C128X64_OX32_OY16 = "jpegtran_800x530_c128x64ox32oy16.jpg";

    // jpegtran -trim -rotate 90 -crop 128x64+32+16 800x530_q75_baseline.jpg >
    // jpegtran_800x530_c128x64ox32oy16.jpg
    String PATH_JPEGTRAN_800x530_ROTATE90_C128X64_OX32_OY16 =
        "jpegtran_800x530_rotate90_c128x64ox32oy16.jpg";

    String PATH_4096x2713_BENCHMARK_LARGE = "4096x2713_benchmark_large.jpg";

    String PATH_16x16_WHITE_Q75 = "16x16_white_q75.jpg";
    String PATH_16x16_BLACK_Q75 = "16x16_black_q75.jpg";
    String PATH_16x16_WHITE_Q75_GRAYSCALE = "16x16_white_q75_grayscale.jpg";
    String PATH_16x16_BLACK_Q75_GRAYSCALE = "16x16_black_q75_grayscale.jpg";
    String PATH_16x16_cABCDF0_Q75 = "16x16_cABCDF0_q75.jpg";
  }

  public interface PNG {
    String PATH_128x85 = "128x85.png";
    String PATH_128x85_ARGB = "128x85_argb.png";
    String PATH_800x530 = "800x530.png";
    String PATH_16x16_ARGB_TRANSPARENT = "16x16_transparent_argb.png";
    String PATH_341x512_R90_CROPPED_MIRRORED = "341x512_r90_cropped_mirrored.png";
  }

  public interface WEBP {
    String PATH_128x85_LOSSLESS = "128x85_rgb_lossless.webp";
    String PATH_128x85_LOSSY = "128x85_rgb_lossy.webp";
    String PATH_16x16_a50_cABCDEF = "16x16_a_50_cABCDEF_lossless.webp";
  }

  public interface IVFAV1 {
    String PATH_256_170_RAV1E_S420_IVFAV1 = "256_170_rav1e_s420.ivf";
    String PATH_256_170_RAV1E_S420_IVFAV1_PNG = "256_170_rav1e_s420.ivf.png";
  }

  public interface GIF {
    String PATH_128x85 = "128x85.gif";
  }

  public static InputStream getInputStream(final String path) throws IOException {
    return getContext().getResources().getAssets().open(path);
  }

  public static byte[] getInputBytes(final String path) throws IOException {
    final InputStream is = getInputStream(path);
    final ByteArrayOutputStream bos = new ByteArrayOutputStream(is.available());

    final byte[] buffer = new byte[16 * 1024];
    int readBytes;
    while ((readBytes = is.read(buffer)) > 0) {
      bos.write(buffer, 0, readBytes);
    }
    return bos.toByteArray();
  }

  static Bitmap getBitmap(final String path) throws IOException {
    InputStream inputStream = null;
    //noinspection TryFinallyCanBeTryWithResources
    try {
      inputStream = getInputStream(path);
      Bitmap bitmap = BitmapFactory.decodeStream(inputStream);
      if (bitmap.getConfig() == null) {
        // HACK: Fixing issue with GIF on Android 7 API 24 where the decoded bitmap is missing
        // bitmap config
        // @see
        // https://android.googlesource.com/platform/frameworks/base/+/android-9.0.0_r8/graphics/java/android/graphics/Bitmap.java#833
        bitmap = bitmap.copy(Bitmap.Config.ARGB_8888, false);
      }
      return bitmap;
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
