/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import android.util.Log;
import com.facebook.spectrum.logging.SpectrumLogcatLogger;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class SpectrumLogcatLoggerTest {

  private static final int[] VALID_LOGCAT_LEVELS =
      new int[] {
        Log.VERBOSE, // = 2
        Log.DEBUG, // = 3
        Log.INFO, // = 4
        Log.WARN, // = 5
        Log.ERROR, // = 6
        Log.ASSERT // = 7
      };

  @Test
  public void testLogcatLogger_whenValidLevel_thenConstructionSucceeds() {
    for (int level : VALID_LOGCAT_LEVELS) {
      new SpectrumLogcatLogger(level);
    }
  }

  @Test(expected = IllegalArgumentException.class)
  public void testLogcatLogger_whenLevelBelowValid_thenThrow() {
    new SpectrumLogcatLogger(1); // lowest valid level is 2
  }

  @Test(expected = IllegalArgumentException.class)
  public void testLogcatLogger_whenLevelAboveValid_thenThrow() {
    new SpectrumLogcatLogger(8); // highest valid level is 7
  }
}
