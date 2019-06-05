/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import static org.fest.assertions.api.Assertions.assertThat;
import static org.junit.Assert.fail;

import android.annotation.SuppressLint;
import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniSpectrumExceptionTest {

  private static final String EXCEPTION_NAME = "name";
  private static final String EXCEPTION_MESSAGE = "message";
  private static final String EXCEPTION_FUNCTION = "function";
  private static final int EXCEPTION_LINE = 0;

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test(expected = SpectrumException.class)
  public void testSpectrumException_whenThrown_thenCorrectType() throws SpectrumException {
    throwSpectrumException("", "", "", 0);
  }

  @Test
  public void testSpectrumException_whenThrown_thenContainsFields() {
    try {
      throwSpectrumException(EXCEPTION_NAME, EXCEPTION_MESSAGE, EXCEPTION_FUNCTION, EXCEPTION_LINE);
      fail("expected exception not thrown");
    } catch (final SpectrumException exception) {
      assertThat(exception.getName()).isEqualTo(EXCEPTION_NAME);
      assertThat(exception.getMessage()).isEqualTo(EXCEPTION_MESSAGE);

      assertThat(exception.getLocation()).contains(EXCEPTION_FUNCTION);
      assertThat(exception.getLocation()).contains(String.valueOf(EXCEPTION_LINE));

      assertThat(exception.getDescription()).contains(EXCEPTION_NAME);
      assertThat(exception.getDescription()).contains(EXCEPTION_MESSAGE);
      assertThat(exception.getDescription()).contains(exception.getLocation());
    }
  }

  @Test
  public void testSpectrumException_whenThrownWithoutMessage_thenContainsFields() {
    try {
      throwSpectrumExceptionWithoutMessage(EXCEPTION_NAME, EXCEPTION_FUNCTION, EXCEPTION_LINE);
      fail("expected exception not thrown");
    } catch (final SpectrumException exception) {
      assertThat(exception.getName()).isEqualTo(EXCEPTION_NAME);
      assertThat(exception.getMessage()).isEqualTo(EXCEPTION_NAME);

      assertThat(exception.getLocation()).contains(EXCEPTION_FUNCTION);
      assertThat(exception.getLocation()).contains(String.valueOf(EXCEPTION_LINE));

      assertThat(exception.getDescription()).contains(EXCEPTION_NAME);
      assertThat(exception.getDescription()).contains(exception.getLocation());
    }
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native void throwSpectrumException(
      final String name, final String message, final String function, final int line)
      throws SpectrumException;

  @DoNotStrip
  private native void throwSpectrumExceptionWithoutMessage(
      final String name, final String function, final int line) throws SpectrumException;
}
