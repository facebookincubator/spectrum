/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.options;

import static org.fest.assertions.api.Assertions.assertThat;

import android.annotation.SuppressLint;
import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.image.ImageSize;
import com.facebook.spectrum.requirements.ResizeRequirement;
import com.facebook.spectrum.requirements.RotateRequirement;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniTransformationsTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenAllNullTransformations_thenEqualIsReturned() {
    final Transformations transformations = new Transformations(null, null, null);
    internalTestTransformationLoopback(transformations);
  }

  @Test
  public void test_whenComplexTransformations_thenEqualIsReturned() {
    final Transformations transformations =
        new Transformations(
            new ResizeRequirement(ResizeRequirement.Mode.EXACT, new ImageSize(640, 480)),
            null,
            new RotateRequirement(90, true, false, true));
    internalTestTransformationLoopback(transformations);
  }

  private void internalTestTransformationLoopback(Transformations transformations) {
    final Transformations twin = loopback(transformations);
    assertThat(twin).isEqualTo(transformations);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native Transformations loopback(Transformations format);
}
