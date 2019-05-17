/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.util.Log;
import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.options.EncodeOptions;
import com.facebook.spectrum.options.TransformOptions;
import com.facebook.spectrum.plugins.SpectrumPlugin;
import com.facebook.spectrum.utils.Preconditions;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Locale;
import javax.annotation.Nullable;

/**
 * Internal object that maintains the state of the native implementation and controls the JNI
 * interaction.
 *
 * <p>Do not use this class directly, but instead use {@link Spectrum}.
 */
@SuppressLint("MissingNativeLoadLibrary")
@DoNotStrip
public class SpectrumHybrid {

  private static final String TAG = "SpectrumHybrid";

  private final Configuration mConfiguration;
  private final SpectrumPlugin[] mPlugins;

  @DoNotStrip @Nullable private HybridData mHybridData;

  public SpectrumHybrid(final Configuration configuration, final SpectrumPlugin[] plugins) {
    mConfiguration = Preconditions.checkNotNull(configuration);
    mPlugins = Preconditions.checkNotNull(plugins);
  }

  /** Returns true if the native library is loaded and initialized properly. */
  @SuppressLint("CatchGeneralException")
  public boolean isAvailable() {
    try {
      ensureNativeLibraryInitialized();
      return mHybridData != null && mHybridData.isValid();
    } catch (Throwable t) {
      Log.e(TAG, String.format((Locale) null, "Failed to load and initialize native: %s", t.getMessage()), t);
      return false;
    }
  }

  /** Wraps the native decode method */
  public SpectrumResult decode(
      final InputStream inputStream,
      final BitmapTarget outputBitmapTarget,
      final DecodeOptions decodeOptions)
      throws SpectrumException {
    ensureNativeLibraryInitialized();
    return nativeDecode(inputStream, outputBitmapTarget, decodeOptions);
  }

  /** Wraps the native encode method */
  public SpectrumResult encode(
      final Bitmap inputBitmap, final OutputStream outputStream, final EncodeOptions encodeOptions)
      throws SpectrumException {
    ensureNativeLibraryInitialized();
    return nativeEncode(inputBitmap, outputStream, encodeOptions);
  }

  /** Wraps the native transcode method */
  public SpectrumResult transcode(
      final InputStream inputStream,
      final OutputStream outputStream,
      final com.facebook.spectrum.options.TranscodeOptions transcodeOptions)
      throws SpectrumException {
    ensureNativeLibraryInitialized();
    return nativeTranscode(inputStream, outputStream, transcodeOptions);
  }

  /** Wraps the native transform method */
  public SpectrumResult transform(
      final Bitmap inputBitmap,
      final BitmapTarget outputBitmapTarget,
      final TransformOptions transformOptions)
      throws SpectrumException {
    ensureNativeLibraryInitialized();
    return nativeTransform(inputBitmap, outputBitmapTarget, transformOptions);
  }

  /* package */ @SuppressWarnings("WeakerAccess")
  synchronized void ensureNativeLibraryInitialized() {
    if (mHybridData != null && mHybridData.isValid()) {
      return;
    }

    SpectrumSoLoader.loadLibrary("spectrum");
    mHybridData = initHybrid(mConfiguration, mPlugins);
  }

  @DoNotStrip
  private native HybridData initHybrid(
      final Configuration configuration, final SpectrumPlugin plugins[]);

  @DoNotStrip
  private native SpectrumResult nativeDecode(
      final InputStream inputStream,
      final BitmapTarget outputBitmapTarget,
      final DecodeOptions decodeOptions)
      throws SpectrumException;

  @DoNotStrip
  private native SpectrumResult nativeEncode(
      final Bitmap inputBitmap, final OutputStream outputStream, final EncodeOptions encodeOptions)
      throws SpectrumException;

  @DoNotStrip
  private native SpectrumResult nativeTranscode(
      final InputStream inputStream,
      final OutputStream outputStream,
      final com.facebook.spectrum.options.TranscodeOptions transcodeOptions)
      throws SpectrumException;

  @DoNotStrip
  private native SpectrumResult nativeTransform(
      final Bitmap inputBitmap,
      final BitmapTarget outputBitmapTarget,
      final TransformOptions transformOptions)
      throws SpectrumException;
}
