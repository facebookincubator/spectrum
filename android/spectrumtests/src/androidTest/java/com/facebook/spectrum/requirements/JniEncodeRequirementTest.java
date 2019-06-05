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
import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniEncodeRequirementTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenLosslessEncodeRequirement_thenEqualIsReturned() {
    testLoopback(new EncodeRequirement(EncodedImageFormat.JPEG));
  }

  @Test
  public void test_whenLossyEncodeRequirement_thenEqualIsReturned() {
    testLoopback(new EncodeRequirement(EncodedImageFormat.JPEG, 42, EncodeRequirement.Mode.LOSSY));
  }

  @Test
  public void test_whenAnyEncodeRequirement_thenEqualIsReturned() {
    testLoopback(new EncodeRequirement(EncodedImageFormat.JPEG, 42, EncodeRequirement.Mode.ANY));
  }

  private void testLoopback(EncodeRequirement original) {
    final EncodeRequirement twin = loopback(original);
    assertThat(twin).isEqualTo(original);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native EncodeRequirement loopback(EncodeRequirement encodeRequirement);
}
