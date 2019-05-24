/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.webp;

import com.facebook.spectrum.DefaultPlugins;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.testutils.SpectrumAssertUtils;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class WebpDecodeTest {

  private Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum = Spectrum.make(new BaseSpectrumLogger(), DefaultPlugins.get());
  }

  @Test
  public void testDecode_whenInputLossy_thenResultMatches() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.WEBP.PATH_128x85_LOSSY)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.WEBP.PATH_128x85_LOSSY));
  }

  @Test
  public void testDecode_whenInputLossless_thenResultMatches() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.WEBP.PATH_128x85_LOSSLESS)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.WEBP.PATH_128x85_LOSSLESS));
  }

  @Test
  public void testDecode_whenInputArgb_thenResultMatches() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.WEBP.PATH_16x16_a50_cABCDEF)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.WEBP.PATH_16x16_a50_cABCDEF));
  }
}
