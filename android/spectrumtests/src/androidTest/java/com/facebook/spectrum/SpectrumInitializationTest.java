/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import static org.fest.assertions.api.Assertions.assertThat;

import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class SpectrumInitializationTest {

  @Before
  public void setup() {
    TestSoLoader.init();
  }

  @Test
  public void testDefaultCreation_whenInitialized_thenNoErrors() {
    final Spectrum spectrum = Spectrum.make(new BaseSpectrumLogger(), DefaultPlugins.get());
    assertThat(spectrum).isNotNull();
  }
}
