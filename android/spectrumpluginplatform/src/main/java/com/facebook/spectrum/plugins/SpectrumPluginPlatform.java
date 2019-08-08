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

/**
 * Decoding plugin that uses the systems {@link android.graphics.BitmapFactory} and supports image
 * formats listed in {@link
 * com.facebook.spectrum.plugins.SpectrumPlatformDecompressor.ImageMimetype}. However, it generally
 * is less efficient than the dedicated plugins (e.g. Jpeg). Therefore, it should be loaded last to
 * give preferences to the native decompressors.
 */
public class SpectrumPluginPlatform extends SpectrumPlugin {

  private static SpectrumPluginPlatform sInstance;

  @DoNotStrip @Nullable private HybridData mHybridData;

  @Override
  protected long createPlugin() {
    return nativeCreatePlugin();
  }

  protected synchronized void ensureLoadedAndInitialized() {
    if (mHybridData != null && mHybridData.isValid()) {
      return;
    }

    SpectrumSoLoader.loadLibrary("spectrumpluginplatform");
    mHybridData = initHybrid();
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native long nativeCreatePlugin();

  /** Returns global instance of the {@link SpectrumPluginPlatform} */
  public static SpectrumPlugin get() {
    synchronized (SpectrumPluginPlatform.class) {
      if (sInstance != null) {
        return sInstance;
      }
      sInstance = new SpectrumPluginPlatform();
      sInstance.ensureLoadedAndInitialized();
      return sInstance;
    }
  }
}
