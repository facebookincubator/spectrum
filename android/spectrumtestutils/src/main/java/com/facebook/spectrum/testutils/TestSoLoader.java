/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import androidx.test.InstrumentationRegistry;
import com.facebook.spectrum.SpectrumSoLoader;
import java.util.concurrent.atomic.AtomicBoolean;

public class TestSoLoader {

  private static AtomicBoolean sAlreadyInitialized = new AtomicBoolean(false);

  public static void init() {
    if (sAlreadyInitialized.getAndSet(true)) {
      // In test environments it is more convenient to call init() before each test case and only
      // have the first one be effective. This is safe as we use the same configuration in every
      // call.
      return;
    }
    SpectrumSoLoader.init(
        InstrumentationRegistry.getContext(), new SpectrumSoLoader.FacebookSoLoaderImpl());
  }

  public static void loadLibrary(final String shortName) {
    SpectrumSoLoader.loadLibrary(shortName);
  }
}
