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
public class JniImageColorTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenGivenImageColor_thenEqualIsReturned() {
    final ImageColor imageColor = new ImageColor(12, 34, 56);
    final ImageColor twin = loopback(imageColor);

    assertThat(twin).isEqualTo(imageColor);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native ImageColor loopback(ImageColor imageColor);
}
