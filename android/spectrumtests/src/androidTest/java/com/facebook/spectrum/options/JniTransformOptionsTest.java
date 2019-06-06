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
import com.facebook.spectrum.image.ImagePixelSpecification;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniTransformOptionsTest extends BaseJniOptionsTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenGiven_thenEqualsReturned() {
    final TransformOptions original =
        TransformOptions.Builder(ImagePixelSpecification.RGB)
            .configuration(TEST_CONFIGURATION)
            .rotate(TEST_ROTATE_REQUIREMENT)
            .resize(TEST_RESIZE_REQUIREMENT)
            .build();

    final Options twin = loopback(original);
    assertThat(twin).isEqualTo(original);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native Options loopback(final TransformOptions transformOptions);
}
