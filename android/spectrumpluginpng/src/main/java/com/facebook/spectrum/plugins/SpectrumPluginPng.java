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

public class SpectrumPluginPng extends SpectrumPlugin {

  private static SpectrumPluginPng sInstance;

  @DoNotStrip @Nullable private HybridData mHybridData;

  @Override
  protected long createPlugin() {
    return nativeCreatePlugin();
  }

  protected synchronized void ensureLoadedAndInitialized() {
    if (mHybridData != null && mHybridData.isValid()) {
      return;
    }

    SpectrumSoLoader.loadLibrary("spectrumpluginpng");
    mHybridData = initHybrid();
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native long nativeCreatePlugin();

  /** Returns global instance of the {@link SpectrumPluginPng} */
  public static SpectrumPlugin get() {
    synchronized (SpectrumPluginPng.class) {
      if (sInstance != null) {
        return sInstance;
      }
      sInstance = new SpectrumPluginPng();
      sInstance.ensureLoadedAndInitialized();
      return sInstance;
    }
  }
}
