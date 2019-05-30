/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins;

import com.facebook.spectrum.DefaultPlugins;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.SpectrumException;
import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import java.io.IOException;
import org.junit.Before;
import org.junit.Test;

/** The WebP tests are separated to exclude them from internal builds */
public class NullTranscodingWebpTest extends NullTranscodingTest {

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum = Spectrum.make(new BaseSpectrumLogger(), DefaultPlugins.get());
  }

  @Test
  public void testNullTranscoding_whenWebpWebp_thenCopied() throws SpectrumException, IOException {
    assertNullTranscoding(TestData.WEBP.PATH_128x85_LOSSY, EncodedImageFormat.WEBP);
  }
}
