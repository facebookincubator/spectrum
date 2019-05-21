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
