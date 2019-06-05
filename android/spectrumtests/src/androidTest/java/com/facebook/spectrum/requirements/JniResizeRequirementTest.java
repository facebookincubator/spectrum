/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.requirements;

import static org.fest.assertions.api.Assertions.assertThat;

import android.annotation.SuppressLint;
import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.image.ImageSize;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniResizeRequirementTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenResizeExact_thenEqualsReturned() {
    testLoopback(new ResizeRequirement(ResizeRequirement.Mode.EXACT, new ImageSize(480, 320)));
  }

  @Test
  public void test_whenResizeExactOrSmaller_thenEqualsReturned() {
    testLoopback(
        new ResizeRequirement(ResizeRequirement.Mode.EXACT_OR_SMALLER, new ImageSize(480, 320)));
  }

  private void testLoopback(ResizeRequirement original) {
    final ResizeRequirement twin = loopback(original);
    assertThat(twin).isEqualTo(original);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native ResizeRequirement loopback(ResizeRequirement resizeRequirement);
}
