/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins;

import static org.fest.assertions.api.Assertions.assertThat;

import com.facebook.spectrum.DefaultPlugins;
import com.facebook.spectrum.EncodedImageSink;
import com.facebook.spectrum.EncodedImageSource;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.SpectrumException;
import com.facebook.spectrum.SpectrumResult;
import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.TranscodeOptions;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import org.junit.Before;
import org.junit.Test;

public class NullTranscodingTest {

  protected Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum = Spectrum.make(new BaseSpectrumLogger(), DefaultPlugins.get());
  }

  @Test
  public void testNullTranscoding_whenJpegJpeg_thenCopied() throws SpectrumException, IOException {
    assertNullTranscoding(TestData.PNG.PATH_128x85, EncodedImageFormat.PNG);
  }

  @Test
  public void testNullTranscoding_whenPngPng_thenCopied() throws SpectrumException, IOException {
    assertNullTranscoding(TestData.JPEG.PATH_128x85_Q75_PROGRESSIVE, EncodedImageFormat.JPEG);
  }

  protected void assertNullTranscoding(
      final String testDataPath, final EncodedImageFormat imageFormat)
      throws IOException, SpectrumException {
    final InputStream inputStream = TestData.getInputStream(testDataPath);
    final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

    final SpectrumResult spectrumResult =
        mSpectrum.transcode(
            EncodedImageSource.from(inputStream),
            EncodedImageSink.from(outputStream),
            TranscodeOptions.Builder(new EncodeRequirement(imageFormat)).build(),
            "callercontext");

    // check metadata
    assertThat(spectrumResult.isSuccessful()).isTrue();
    assertThat(spectrumResult.getRuleName()).isEqualTo("copy");

    // check equality byte-for-byte
    assertThat(outputStream.toByteArray()).isEqualTo(TestData.getInputBytes(testDataPath));
  }
}
