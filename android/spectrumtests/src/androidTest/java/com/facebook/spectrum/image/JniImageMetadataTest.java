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
public class JniImageMetadataTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenGivenMetadata_thenEqualIsReturned() {
    final ImageMetadata metadata = new ImageMetadata();
    final ImageMetadata twin = loopback(metadata);

    assertThat(twin).isEqualTo(metadata);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native ImageMetadata loopback(ImageMetadata metadata);
}
