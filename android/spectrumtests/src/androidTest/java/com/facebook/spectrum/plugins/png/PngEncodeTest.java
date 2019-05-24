/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.png;

import static com.facebook.spectrum.image.EncodedImageFormat.PNG;

import com.facebook.spectrum.Configuration;
import com.facebook.spectrum.DefaultPlugins;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.EncodeOptions;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.testutils.SpectrumAssertUtils;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class PngEncodeTest {

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
            .encoding(EncodeOptions.Builder(new EncodeRequirement(PNG)).build())
            .assertingOutputFormat(EncodedImageFormat.PNG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE));
  }

  @Test
  public void testEncode_whenInterlacing_thenResultSimilar() throws Exception {
    final Configuration configuration = Configuration.Builder().setUseInterlacing(true).build();
    final EncodeOptions encodeOptions =
        EncodeOptions.Builder(new EncodeRequirement(PNG)).configuration(configuration).build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .encoding(encodeOptions)
            .assertingOutputFormat(EncodedImageFormat.PNG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE));
  }
}
