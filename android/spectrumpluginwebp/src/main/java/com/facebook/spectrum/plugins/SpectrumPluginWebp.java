/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins;

import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.SpectrumSoLoader;
import javax.annotation.Nullable;

public class SpectrumPluginWebp extends SpectrumPlugin {

  private static SpectrumPluginWebp sInstance;

  @DoNotStrip @Nullable private HybridData mHybridData;

  @Override
  protected long createPlugin() {
    return nativeCreatePlugin();
  }

  protected synchronized void ensureLoadedAndInitialized() {
    if (mHybridData != null && mHybridData.isValid()) {
      return;
    }

    SpectrumSoLoader.loadLibrary("spectrumpluginwebp");
    mHybridData = initHybrid();
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native long nativeCreatePlugin();

  /** Returns global instance of the {@link SpectrumPluginWebp} */
  public static SpectrumPlugin get() {
    synchronized (SpectrumPluginWebp.class) {
      if (sInstance != null) {
        return sInstance;
      }
      sInstance = new SpectrumPluginWebp();
      sInstance.ensureLoadedAndInitialized();
      return sInstance;
    }
  }
}
