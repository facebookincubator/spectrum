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

public class SpectrumPluginAvif extends SpectrumPlugin {

  private static SpectrumPluginAvif sInstance;

  @DoNotStrip @Nullable private HybridData mHybridData;

  @Override
  protected long createPlugin() {
    return nativeCreatePlugin();
  }

  protected synchronized void ensureLoadedAndInitialized() {
    if (mHybridData != null && mHybridData.isValid()) {
      return;
    }

    SpectrumSoLoader.loadLibrary("spectrumpluginavif");
    mHybridData = initHybrid();
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native long nativeCreatePlugin();

  /** Returns global instance of the {@link SpectrumPluginAvif} */
  public static SpectrumPlugin get() {
    synchronized (SpectrumPluginAvif.class) {
      if (sInstance != null) {
        return sInstance;
      }
      sInstance = new SpectrumPluginAvif();
      sInstance.ensureLoadedAndInitialized();
      return sInstance;
    }
  }
}
