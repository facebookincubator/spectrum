// Copyright 2004-present Facebook. All Rights Reserved.

package com.facebook.spectrum.image;

import static org.fest.assertions.api.Assertions.assertThat;

import android.annotation.SuppressLint;
import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniImageFormatTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenGivenImageFormat_thenEqualIsReturned() {
    testLoopback(ImageFormat.BITMAP);
  }

  @Test
  public void test_whenEncodedImageFormat_thenEqualIsReturned() {
    testLoopback(EncodedImageFormat.JPEG);
  }

  private void testLoopback(ImageFormat original) {
    final ImageFormat twin = loopback(original);
    assertThat(twin).isEqualTo(original);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native ImageFormat loopback(ImageFormat format);
}
