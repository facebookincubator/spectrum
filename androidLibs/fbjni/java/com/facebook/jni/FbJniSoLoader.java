/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */
package com.facebook.jni;

/** SoLoader delegate to allow making FbJni use the SpectrumSoLoader */
public final class FbJniSoLoader {

  public static SoLoaderWrapper delegate = null;

  private FbJniSoLoader() {}

  public static void loadLibrary(final String shortName) {
    delegate.loadLibrary(shortName);
  }

  public interface SoLoaderWrapper {
    void loadLibrary(final String shortName);
  }
}
