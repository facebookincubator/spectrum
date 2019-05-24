/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.png;

import static com.facebook.spectrum.image.EncodedImageFormat.PNG;

import com.facebook.spectrum.DefaultPlugins;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.image.ImageSize;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.TranscodeOptions;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.requirements.ResizeRequirement;
import com.facebook.spectrum.testutils.SpectrumAssertUtils;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class PngTranscodeTest {

  private Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum = Spectrum.make(new BaseSpectrumLogger(), DefaultPlugins.get());
  }

  @Test
  public void testTranscode_whenCanPassthrough_thenPerfectOutput() throws Exception {
    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(PNG)).build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.PNG.PATH_128x85)
            .transcoding(transcodeOptions)
            .assertingOutputFormat(PNG)
            .comparingAgainstTestFile(TestData.PNG.PATH_128x85)
            .usingSsimTarget(1f)
            .usingSsimTolerance(0f));
  }

  @Test
  public void testTranscode_whenScaledDown_thenMatchesExpected() throws Exception {
    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(PNG))
            .resize(ResizeRequirement.Mode.EXACT, new ImageSize(128, 128))
            .build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.PNG.PATH_800x530)
            .transcoding(transcodeOptions)
            .assertingOutputFormat(PNG)
            .comparingAgainstTestFile(TestData.PNG.PATH_128x85)
            .usingSsimTarget(.88f));
  }
}
