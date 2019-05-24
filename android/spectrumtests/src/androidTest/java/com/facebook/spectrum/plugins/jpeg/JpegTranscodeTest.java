/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.jpeg;

import static com.facebook.spectrum.image.EncodedImageFormat.JPEG;
import static com.facebook.spectrum.requirements.EncodeRequirement.Mode.LOSSY;

import android.media.ExifInterface;
import com.facebook.spectrum.Configuration;
import com.facebook.spectrum.DefaultPlugins;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.TranscodeOptions;
import com.facebook.spectrum.requirements.CropRequirement;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.testutils.SpectrumAssertUtils;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class JpegTranscodeTest {

  private Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum = Spectrum.make(new BaseSpectrumLogger(), DefaultPlugins.get());
  }

  @Test
  public void testTranscode_whenCanPassthrough_thenPerfectOutput() throws Exception {
    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(JPEG)).build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .transcoding(transcodeOptions)
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .usingSsimTarget(1f)
            .usingSsimTolerance(0f));
  }

  @Test
  public void testTranscode_whenReEncodingHighQuality_thenHighScore() throws Exception {
    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(JPEG, 95, LOSSY)).build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .transcoding(transcodeOptions)
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .usingSsimTarget(0.95f));
  }

  @Test
  public void testTranscode_whenReEncodingVeryLowQuality_thenLowScore() throws Exception {
    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(JPEG, 5, LOSSY)).build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .transcoding(transcodeOptions)
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_BASELINE)
            .usingSsimTarget(0.65f)
            .usingSsimTolerance(0.1f));
  }

  @Test
  public void testTranscode_whenAbsoluteCropping_thenSimilarToKnownResult() throws Exception {
    final int l = 100;
    final int t = 50;
    final int w = 200;
    final int h = 100;

    final CropRequirement cropRequirement =
        CropRequirement.makeAbsoluteToOrigin(l, t, l + w, t + h, true);

    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(JPEG, 95)).crop(cropRequirement).build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_800x530_Q75_PROGRESSIVE)
            .transcoding(transcodeOptions)
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_CROPPED_200x100_100_50_Q95));
  }

  @Test
  public void testTranscode_whenRelativeCropping_thenSimilarToKnownResult() throws Exception {
    final float w = 0.5f;
    final float h = 0.6f;
    final float l = 0.5f - w / 2f;
    final float t = 0.5f - h / 2f;

    final CropRequirement cropRequirement =
        CropRequirement.makeRelativeToOrigin(l, t, l + w, t + h, true);

    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(JPEG, 95)).crop(cropRequirement).build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_800x530_Q75_PROGRESSIVE)
            .transcoding(transcodeOptions)
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_CROPPED_RELATIVE_50Px60P_CENTER_Q95));
  }

  @Test
  public void testTranscode_whenRotated90WithMetadata_thenVerySimilarToRotatedVersion()
      throws Exception {
    final Configuration configuration = Configuration.Builder().setInterpretMetadata(true).build();

    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(JPEG, 95, LOSSY))
            .configuration(configuration)
            .rotate(90)
            .build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_PROGRESSIVE)
            .transcoding(transcodeOptions)
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_PROGRESSIVE)
            .assertingOutputExifOrientation(ExifInterface.ORIENTATION_ROTATE_90));
  }

  @Test
  public void testTranscode_whenRotated90WithoutMetadata_thenVerySimilarToRotatedVersion()
      throws Exception {
    final Configuration configuration = Configuration.Builder().setInterpretMetadata(false).build();

    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(JPEG, 95, LOSSY))
            .configuration(configuration)
            .rotate(90)
            .build();

    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.JPEG.PATH_128x85_Q75_PROGRESSIVE)
            .transcoding(transcodeOptions)
            .assertingOutputFormat(JPEG)
            .comparingAgainstTestFile(TestData.JPEG.PATH_128x85_Q75_PROGRESSIVE_R90)
            .assertingOutputExifOrientation(ExifInterface.ORIENTATION_UNDEFINED));
  }
}
