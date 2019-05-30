/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins.ivfav1;

import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.logging.BaseSpectrumLogger;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.plugins.SpectrumPlugin;
import com.facebook.spectrum.plugins.SpectrumPluginAvif;
import com.facebook.spectrum.testutils.SpectrumAssertUtils;
import com.facebook.spectrum.testutils.TestData;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

public class IvfAv1DecodeTest {

  private Spectrum mSpectrum;

  @Before
  public void setup() {
    TestSoLoader.init();
    mSpectrum =
        Spectrum.make(new BaseSpectrumLogger(), new SpectrumPlugin[] {SpectrumPluginAvif.get()});
  }

  @Test
  public void testDecode_whenInputAvif_thenResultMatches() throws Exception {
    SpectrumAssertUtils.executeAndAssert(
        mSpectrum,
        SpectrumAssertUtils.Builder.withTestImage(TestData.IVFAV1.PATH_256_170_RAV1E_S420_IVFAV1)
            .decoding(DecodeOptions.Builder().build())
            .comparingAgainstTestFile(TestData.IVFAV1.PATH_256_170_RAV1E_S420_IVFAV1_PNG));
  }
}
