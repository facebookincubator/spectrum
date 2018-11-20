/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins;

import android.util.Log;
import com.facebook.jni.annotations.DoNotStrip;
import java.util.Locale;

/**
 * Common plugin interface that allow aggregating native plugin creation methods between modules.
 */
@DoNotStrip
public abstract class SpectrumPlugin {

  private static final String TAG = "SpectrumPlugin";

  /**
   * Creates a new native instance of the plugin. The caller is responsible to subsequently free the
   * object by a call to the native <code>free(...)</code> C++ call as this will not be performed
   * automatically.
   */
  @DoNotStrip
  public final synchronized long getPlugin() {
    ensureLoadedAndInitialized();

    final long nativePluginPointer = createPlugin();
    Log.d(TAG, String.format((Locale) null, "Created plugin at 0x%016X", nativePluginPointer));

    return nativePluginPointer;
  }

  /** Creates a new native plugin on the heap and returns a raw pointer */
  protected abstract long createPlugin();

  /** Ensures the native library is loaded and the hybrid object is initialized */
  protected abstract void ensureLoadedAndInitialized();
}
