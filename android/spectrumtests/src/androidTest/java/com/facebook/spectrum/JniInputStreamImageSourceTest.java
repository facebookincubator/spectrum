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
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniInputStreamImageSourceTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @SuppressLint("CatchGeneralException")
  @Test(expected = RuntimeException.class)
  public void testJniSource_whenNull_thenException() {
    try {
      readFromInputStream(null);
    } catch (RuntimeException e) {
      assertThat(e.getMessage()).contains("null");
      assertThat(e.getMessage()).containsIgnoringCase("inputstream");
      throw e;
    }
  }

  @Test
  public void testJniSource_whenEmpty_thenEmptyResult() throws IOException {
    final InputStream is = createInputStreamFromString("");
    final String actual = readFromInputStreamToString(is);

    assertThat(actual).isEmpty();
    assertThat(is.available()).isEqualTo(0);

    is.close();
  }

  @Test
  public void testJniSource_whenSampleContent_thenResultEqualsContent() throws IOException {
    final String sampleContent = "sample content";

    final InputStream is = createInputStreamFromString(sampleContent);
    final String actual = readFromInputStreamToString(is);

    assertThat(actual).isEqualTo(sampleContent);
    assertThat(is.available()).isEqualTo(0);

    is.close();
  }

  @Test
  public void testJniSource_whenBufferedInputStreamAndSampleContent_thenResultEqualsContent()
      throws IOException {
    final String sampleContent = "sample content";

    final BufferedInputStream bufferedInputStream =
        new BufferedInputStream(createInputStreamFromString(sampleContent));
    final String actual = readFromInputStreamToString(bufferedInputStream);

    assertThat(actual).isEqualTo(sampleContent);
    assertThat(bufferedInputStream.available()).isEqualTo(0);

    bufferedInputStream.close();
  }

  @Test
  public void testJniSource_whenVeryLongContent_thenResultEqualsContent() throws IOException {
    final StringBuilder sb = new StringBuilder(1000 * 26);
    for (int i = 0; i < 1000; i++) {
      sb.append("abcdefghijklmnopqrstuvwxyz");
    }
    final String veryLongContent = sb.toString();

    final InputStream is = createInputStreamFromString(veryLongContent);
    final String actual = readFromInputStreamToString(is);

    assertThat(actual).isEqualTo(veryLongContent);
    assertThat(is.available()).isEqualTo(0);

    is.close();
  }

  @Test
  public void testJniSource_whenEntireByteRange_thenResultEqualsContent() throws IOException {
    byte[] content = new byte[255];
    for (int i = 0; i < 255; i++) {
      content[i] = (byte) (Byte.MIN_VALUE + i);
    }

    final InputStream is = createInputStream(content);
    final byte[] actual = readFromInputStream(is);

    assertThat(actual).isEqualTo(content);
    assertThat(is.available()).isEqualTo(0);

    is.close();
  }

  private static InputStream createInputStream(final byte[] arr) {
    return new ByteArrayInputStream(arr);
  }

  private static InputStream createInputStreamFromString(String s) {
    return createInputStream(s.getBytes());
  }

  private String readFromInputStreamToString(final InputStream inputStream) {
    return new String(readFromInputStream(inputStream));
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native byte[] readFromInputStream(final InputStream inputStream);

  @DoNotStrip
  private native int getAvailableFromInputStream(final InputStream inputStream);
}
