/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import static org.fest.assertions.api.Assertions.assertThat;

import android.annotation.SuppressLint;
import androidx.test.runner.AndroidJUnit4;
import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

@SuppressLint("MissingNativeLoadLibrary")
@RunWith(AndroidJUnit4.class)
public class JniImageOrientationTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenGivenEveryImageOrientation_thenLoopbackEquals() {
    for (final ImageOrientation imageOrientation : ImageOrientation.values()) {
      final ImageOrientation twin = loopback(imageOrientation);
      assertThat(twin).isEqualTo(imageOrientation);
    }
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native ImageOrientation loopback(ImageOrientation imageOrientation);
}
