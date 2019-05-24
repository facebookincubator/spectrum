/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.webp;

import static com.facebook.spectrum.image.EncodedImageFormat.WEBP;
import static com.facebook.spectrum.requirements.EncodeRequirement.Mode.LOSSLESS;
import static com.facebook.spectrum.requirements.EncodeRequirement.Mode.LOSSY;

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

public class WebpEncodeTest {

  private Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum = Spectrum.make(new BaseSpectrumLogger(), DefaultPlugins.get());
  }

  @Test
  public void testEncode_whenLossy_thenResultSimilar() throws Exception {
    final EncodeRequirement encodeRequirement = new EncodeRequirement(WEBP, 80, LOSSY);

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .encoding(EncodeOptions.Builder(encodeRequirement).build())
            .assertingOutputFormat(WEBP)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE));
  }

  @Test
  public void testEncode_whenLossless_thenResultSimilar() throws Exception {
    final EncodeRequirement encodeRequirement = new EncodeRequirement(WEBP, LOSSLESS);

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .encoding(EncodeOptions.Builder(encodeRequirement).build())
            .assertingOutputFormat(WEBP)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE));
  }
}
