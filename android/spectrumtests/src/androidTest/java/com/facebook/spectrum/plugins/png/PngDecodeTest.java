/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.png;

import com.facebook.spectrum.DefaultPlugins;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.testutils.SpectrumAssertUtils;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class PngDecodeTest {

  private Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum = Spectrum.make(new BaseSpectrumLogger(), DefaultPlugins.get());
  }

  @Test
  public void testDecode_whenInputRgb_thenResultMatches() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.PNG.PATH_128x85)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.PNG.PATH_128x85)
            .usingSsimTarget(0.99f)
            .usingSsimTolerance(0.01f));
  }

  @Test
  public void testDecode_whenInputArgb_thenResultMatches() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.PNG.PATH_128x85_ARGB)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.PNG.PATH_128x85)
            .usingSsimTarget(0.99f)
            .usingSsimTolerance(0.01f));
  }
}
