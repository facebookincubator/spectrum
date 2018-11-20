/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import android.app.Application;
import android.content.Context;
import android.util.Log;
import com.facebook.soloader.SoLoader;
import java.io.IOException;

/**
 * Helper class for initializing the SoLoader library (see https://github.com/facebook/SoLoader).
 */
public class SpectrumSoLoader {

  /**
   * Initializes the SoLoader library. This should be called in you applications {@link
   * Application#onCreate()} method.
   *
   * @param context The application's context
   */
  public static void init(final Context context) {
    try {
      SoLoader.init(context, 0);
    } catch (final IOException e) {
      Log.e("SpectrumSoLoader", "SoLoader.init() failed", e);
    }
  }
}
