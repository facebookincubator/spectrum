/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.jpeg;

import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.plugins.SpectrumPlugin;
import com.facebook.spectrum.plugins.SpectrumPluginJpeg;
import com.facebook.spectrum.testutils.SpectrumAssertUtils;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class JpegDecodeTest {

  private Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum =
        Spectrum.make(new BaseSpectrumLogger(), new SpectrumPlugin[] {SpectrumPluginJpeg.get()});
  }

  @Test
  public void testDecode_whenInputBaseline_thenResultMatches() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .usingSsimTarget(0.99f)
            .usingSsimTolerance(0.01f));
  }

  @Test
  public void testDecode_whenInputProgressive_thenResultMatches() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_PROGRESSIVE)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_PROGRESSIVE)
            .usingSsimTarget(0.99f)
            .usingSsimTolerance(0.01f));
  }
}
