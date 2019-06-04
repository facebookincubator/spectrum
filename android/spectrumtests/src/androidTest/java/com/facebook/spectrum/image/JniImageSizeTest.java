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
public class JniImageSizeTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenGivenImageSize_thenEqualIsReturned() {
    final ImageSize imageSize = new ImageSize(640, 480);
    final ImageSize twin = loopback(imageSize);

    assertThat(twin).isEqualTo(imageSize);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native ImageSize loopback(ImageSize imagePixelSpecification);
}
