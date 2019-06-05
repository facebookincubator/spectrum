/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import static org.fest.assertions.api.Assertions.assertThat;

import android.annotation.SuppressLint;
import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniImagePixelSpecificationTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenGivenImagePixelSpecification_thenEqualIsReturned() {
    final ImagePixelSpecification[] candidates = ImagePixelSpecification.values();

    for (final ImagePixelSpecification imagePixelSpecification : candidates) {
      final ImagePixelSpecification twin = loopback(imagePixelSpecification);
      assertThat(twin).isEqualTo(imagePixelSpecification);
    }
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native ImagePixelSpecification loopback(ImagePixelSpecification imagePixelSpecification);
}
