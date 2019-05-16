/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import androidx.test.InstrumentationRegistry;
import com.facebook.soloader.SoLoader;
import java.io.IOException;

public class TestSoLoader {

  public static void init() {
    try {
      SoLoader.init(InstrumentationRegistry.getContext(), 0);
    } catch (final IOException e) {
      throw new RuntimeException("failed to initialize SoLoader");
    }
  }
}
