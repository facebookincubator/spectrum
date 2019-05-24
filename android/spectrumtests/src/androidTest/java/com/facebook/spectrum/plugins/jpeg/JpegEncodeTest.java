/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.jpeg;

import static com.facebook.spectrum.image.EncodedImageFormat.JPEG;

import com.facebook.spectrum.Configuration;
import com.facebook.spectrum.DefaultPlugins;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.EncodeOptions;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.testutils.SpectrumAssertUtils;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class JpegEncodeTest {

  private Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum = Spectrum.make(new BaseSpectrumLogger(), DefaultPlugins.get());
  }

  @Test
  public void testEncode_whenDefaultOptions_thenResultSimilar() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .encoding(EncodeOptions.Builder(new EncodeRequirement(JPEG)).build())
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE));
  }

  @Test
  public void testEncode_whenBaseline_thenResultSimilar() throws Exception {
    final Configuration configuration = Configuration.Builder().setUseProgressive(false).build();
    final EncodeOptions encodeOptions =
        EncodeOptions.Builder(new EncodeRequirement(JPEG)).configuration(configuration).build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .encoding(encodeOptions)
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE));
  }

  @Test
  public void testEncode_whenNoScanOptimizationAndNoTrellis_thenResultSimilar() throws Exception {
    final Configuration configuration =
        Configuration.Builder().setUseOptimizeScan(false).setUseTrellis(false).build();
    final EncodeOptions encodeOptions =
        EncodeOptions.Builder(new EncodeRequirement(JPEG)).configuration(configuration).build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .encoding(encodeOptions)
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE));
  }
}
