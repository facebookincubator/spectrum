/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import static org.fest.assertions.api.Assertions.assertThat;
import static org.fest.assertions.api.Assertions.fail;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;

import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.logging.SpectrumLogger;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.options.Options;
import com.facebook.spectrum.options.TranscodeOptions;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.testutils.FailingInputStream;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import org.junit.Before;
import org.junit.Test;

public class SpectrumLoggerTest {

  private static final Object CALLER_CONTEXT = "SpectrumLoggerTest";

  private Spectrum mSpectrum;
  private SpectrumLogger mSpectrumLogger;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrumLogger = mock(BaseSpectrumLogger.class);
    mSpectrum = Spectrum.make(mSpectrumLogger, DefaultPlugins.get());
  }

  @Test
  public void testSpectrumLogger_whenTranscoding_thenLoggerCalled()
      throws IOException, SpectrumException {
    final InputStream inputStream =
        TestData.getInputStream(TestData.JPEG.PATH_128x85_Q75_PROGRESSIVE);

    final TranscodeOptions transcodeOptions =
        TranscodeOptions.Builder(new EncodeRequirement(EncodedImageFormat.JPEG)).build();
    mSpectrum.transcode(
        EncodedImageSource.from(inputStream),
        EncodedImageSink.from(new ByteArrayOutputStream()),
        transcodeOptions,
        CALLER_CONTEXT);

    // assertions on logger
    verify(mSpectrumLogger).onStart(any(Options.class), eq(CALLER_CONTEXT));
    verify(mSpectrumLogger).onFinish(any(Object.class), any(SpectrumResult.class));
  }

  @Test
  public void testSpectrumLogger_whenTranscodingFromBadSource_thenLoggerErrorCalled()
      throws IOException {
    final InputStream inputStream =
        TestData.getInputStream(TestData.JPEG.PATH_128x85_Q75_PROGRESSIVE);
    final InputStream badInputStream = new FailingInputStream(inputStream, 10);

    try {
      final DecodeOptions decodeOptions = DecodeOptions.Builder().build();
      mSpectrum.decode(
          EncodedImageSource.from(badInputStream),
          new BitmapTarget(),
          decodeOptions,
          CALLER_CONTEXT);
      fail("did not encounter expected exception");
    } catch (final SpectrumException e) {
      assertThat(e.getCause().getLocalizedMessage())
          .containsIgnoringCase(FailingInputStream.EXCEPTION_MESSAGE);
    } finally {
      badInputStream.close();
      inputStream.close();
    }

    // assertions on logger
    verify(mSpectrumLogger).onStart(any(Options.class), eq(CALLER_CONTEXT));
    verify(mSpectrumLogger).onError(any(Object.class), any(IOException.class));
    verify(mSpectrumLogger).onFinish(any(Object.class), any(SpectrumResult.class));
  }
}
