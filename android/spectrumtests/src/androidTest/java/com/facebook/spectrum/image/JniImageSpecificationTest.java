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
public class JniImageSpecificationTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenGivenInstanceWithMinimalFields_thenLoopbackEqual() {
    final ImageSpecification specification =
        new ImageSpecification(
            new ImageSize(320, 240), ImageFormat.BITMAP, ImagePixelSpecification.RGB);
    final ImageSpecification twin = loopback(specification);
    assertThat(twin).isEqualTo(specification);
  }

  @Test
  public void test_whenGivenInstanceWithAllFields_thenLoopbackEqual() {
    final ImageSpecification specification =
        new ImageSpecification(
            new ImageSize(320, 240),
            EncodedImageFormat.JPEG,
            ImagePixelSpecification.RGB,
            ImageOrientation.LEFT,
            ImageChromaSamplingMode.S420,
            new ImageMetadata());
    final ImageSpecification twin = loopback(specification);
    assertThat(twin).isEqualTo(specification);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native ImageSpecification loopback(ImageSpecification ChromaSamplingMode);
}
