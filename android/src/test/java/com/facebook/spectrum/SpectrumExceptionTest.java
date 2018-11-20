/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import static org.fest.assertions.api.Assertions.assertThat;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class SpectrumExceptionTest {

  public static final String EXCEPTION_MESSAGE = "Flux compensator implosion";
  public static final SpectrumException EXCEPTION_CAUSE = new SpectrumException("time drift");

  @Test
  public void testSpectrumException_whenGivenMessage_thenContainsMessage() {
    final SpectrumException spectrumException = new SpectrumException(EXCEPTION_MESSAGE);
    assertThat(spectrumException.getMessage()).isEqualTo(EXCEPTION_MESSAGE);
  }

  @Test
  public void testSpectrumException_whenGivenCause_thenContainsCause() {
    final SpectrumException spectrumException = new SpectrumException(EXCEPTION_CAUSE);
    assertThat(spectrumException.getCause()).isEqualTo(EXCEPTION_CAUSE);
  }

  @Test
  public void testSpectrumException_whenGivenMessageAndCause_thenContainsMessageAndCause() {
    final SpectrumException spectrumException =
        new SpectrumException(EXCEPTION_MESSAGE, EXCEPTION_CAUSE);
    assertThat(spectrumException.getMessage()).isEqualTo(EXCEPTION_MESSAGE);
    assertThat(spectrumException.getCause()).isEqualTo(EXCEPTION_CAUSE);
  }
}
