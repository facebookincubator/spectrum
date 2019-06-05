/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import static org.fest.assertions.api.Assertions.assertThat;

import android.annotation.SuppressLint;
import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.testutils.TestSoLoader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniOutputStreamImageSinkTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @SuppressLint("CatchGeneralException")
  @Test(expected = RuntimeException.class)
  public void testJniSink_whenNull_thenException() {
    try {
      writeToOutputStream(new byte[0], null);
    } catch (RuntimeException e) {
      assertThat(e.getMessage()).contains("null");
      assertThat(e.getMessage()).containsIgnoringCase("outputstream");
      throw e;
    }
  }

  @Test
  public void testJniSink_whenEmpty_thenEmptyResult() throws IOException {
    final String emptyContent = "";
    final String actual = writeToOutputStreamAndReadBack(emptyContent);
    assertThat(actual).isEmpty();
  }

  @Test
  public void testJniSink_whenSampleContent_thenResultEqualsContent() throws IOException {
    final String sampleContent = "sample content";
    final String actual = writeToOutputStreamAndReadBack(sampleContent);
    assertThat(actual).isEqualTo(sampleContent);
  }

  @Test
  public void testJniSink_whenVeryLongContent_thenResultEqualsContent() throws IOException {
    final StringBuilder sb = new StringBuilder(1000 * 26);
    for (int i = 0; i < 1000; i++) {
      sb.append("abcdefghijklmnopqrstuvwxyz");
    }

    final String veryLongContent = sb.toString();
    final String actual = writeToOutputStreamAndReadBack(veryLongContent);
    assertThat(actual).isEqualTo(veryLongContent);
  }

  @Test
  public void testJniSink_whenEntireByteRange_thenResultEqualsContent() throws IOException {
    byte[] content = new byte[255];
    for (int i = 0; i < 255; i++) {
      content[i] = (byte) (Byte.MIN_VALUE + i);
    }

    final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
    writeToOutputStream(content, outputStream);
    assertThat(outputStream.toByteArray()).isEqualTo(content);
  }

  private String writeToOutputStreamAndReadBack(final String inputString) throws IOException {
    final byte[] inputBytes = inputString.getBytes();

    final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

    writeToOutputStream(inputBytes, outputStream);
    outputStream.close();

    return new String(outputStream.toByteArray());
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native void writeToOutputStream(final byte[] array, final OutputStream outputStream);
}
