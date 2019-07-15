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
import com.facebook.jni.FbJniSoLoader;
import com.facebook.soloader.SoLoader;
import java.io.IOException;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * A Spectrum abstraction allowing to use custom so loaders. It comes with two implementations:
 * {@link FacebookSoLoaderImpl} (default) and {@link SystemSoLoaderImpl}.
 */
public class SpectrumSoLoader {

  /**
   * Double initialization might create unexpected behaviour when .so files are loaded through
   * different channels. Therefore, we don't allow it.
   */
  private static final AtomicBoolean sAlreadyInitialized = new AtomicBoolean(false);

  /** The delegate to use for both {@link #init} and {@link #loadLibrary}. */
  private static SoLoaderWrapper sSoLoaderDelegate;

  /**
   * Initializes the SoLoader library. This should be called in you applications {@link
   * Application#onCreate()} method.
   *
   * @param context The application's context
   * @throws IllegalStateException if called more than once
   */
  public static void init(final Context context) {
    init(context, new FacebookSoLoaderImpl());
  }

  /**
   * Initializes the SoLoader library. This should be called in you applications {@link
   * Application#onCreate()} method.
   *
   * @param context The application's context
   * @throws IllegalStateException if called more than once
   */
  public static void init(final Context context, final SoLoaderWrapper soLoaderDelegate) {
    if (sAlreadyInitialized.getAndSet(true)) {
      throw new IllegalStateException("SpectrumSoLoader must not be initialized more than once");
    }

    sSoLoaderDelegate = soLoaderDelegate;
    sSoLoaderDelegate.init(context);

    // also make our FbJni copy use this SoLoader abstraction
    FbJniSoLoader.delegate = new SpectrumImplForFbJniSoLoaderDelegate();
  }

  public static void loadLibrary(final String shortName) {
    synchronized (sAlreadyInitialized) {
      if (!sAlreadyInitialized.getAndSet(true)) {
        // Default initialize to the default SoLoader assuming it's already initialized. This is to
        // preserve backwards compatibility with v1.0 behaviour and internal usage.
        sSoLoaderDelegate = new FacebookSoLoaderImpl();
      }
    }

    sSoLoaderDelegate.loadLibrary(shortName);
  }

  public interface SoLoaderWrapper {
    void init(final Context context);

    void loadLibrary(final String shortName);
  }

  /** Uses the Facebook SoLoader implementation – see https://github.com/facebook/SoLoader */
  public static class FacebookSoLoaderImpl implements SoLoaderWrapper {

    @Override
    public void init(final Context context) {
      try {
        SoLoader.init(context, 0);
      } catch (final IOException e) {
        Log.e("FacebookSoLoaderWrapper", "SoLoader.init() failed", e);
      }
    }

    @Override
    public void loadLibrary(final String shortName) {
      SoLoader.loadLibrary(shortName);
    }
  }

  /** Uses the system so loader – see {@link System#loadLibrary(String)} */
  public static class SystemSoLoaderImpl implements SoLoaderWrapper {

    @Override
    public void init(final Context context) {
      // intentionally empty, as the system so loader does not require initialization
    }

    @Override
    public void loadLibrary(final String shortName) {
      System.loadLibrary(shortName);
    }
  }

  /**
   * Delegate implementation for the {@link FbJniSoLoader} to use this {@link SpectrumSoLoader}.
   * Required to avoid circular dependencies between modules.
   */
  private static final class SpectrumImplForFbJniSoLoaderDelegate
      implements FbJniSoLoader.SoLoaderWrapper {

    @Override
    public void loadLibrary(final String shortName) {
      SpectrumSoLoader.loadLibrary(shortName);
    }
  }
}
