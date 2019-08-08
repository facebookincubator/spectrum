/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.platform;

import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.plugins.SpectrumPlugin;
import com.facebook.spectrum.plugins.SpectrumPluginPlatform;
import com.facebook.spectrum.testutils.SpectrumAssertUtils;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class PlatformDecodeTest {

  private Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum =
        Spectrum.make(
            new BaseSpectrumLogger(), new SpectrumPlugin[] {SpectrumPluginPlatform.get()});
  }

  @Test
  public void testDecode_whenInputPng_thenDecodesProperly() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.PNG.PATH_128x85)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.PNG.PATH_128x85)
            .usingSsimTarget(0.99f)
            .usingSsimTolerance(0.01f));
  }

  @Test
  public void testDecode_whenInputJpeg_thenDecodesProperly() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .usingSsimTarget(0.99f)
            .usingSsimTolerance(0.01f));
  }

  @Test
  public void testDecode_whenInputWebp_thenDecodesProperly() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.WEBP.PATH_128x85_LOSSLESS)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.WEBP.PATH_128x85_LOSSLESS)
            .usingSsimTarget(0.99f)
            .usingSsimTolerance(0.01f));
  }

  @Test
  public void testDecode_whenInputGif_thenDecodesProperly() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.GIF.PATH_128x85)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.GIF.PATH_128x85)
            .usingSsimTarget(0.99f)
            .usingSsimTolerance(0.01f));
  }
}
