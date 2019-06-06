/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.types;

import static org.fest.assertions.api.Assertions.assertThat;

import android.annotation.SuppressLint;
import com.facebook.jni.HybridData;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.Configuration;
import com.facebook.spectrum.image.ImageChromaSamplingMode;
import com.facebook.spectrum.image.ImageColor;
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniConfigurationTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenConfigurationAllEmpty_thenEqualIsReturned() {
    testLoopback(Configuration.Builder().build());
  }

  @Test
  public void test_whenConfigurationWithEverythingSet_thenEqualIsReturned() {
    final Configuration configuration =
        Configuration.Builder()
            .setDefaultBackgroundColor(new ImageColor(1, 2, 3))
            .setInterpretMetadata(true)
            .setPropagateChromaSamplingModeFromSource(true)
            .setUseTrellis(true)
            .setUseProgressive(true)
            .setUseOptimizeScan(true)
            .setUseCompatibleDcScanOpt(true)
            .setChromaSamplingModeOverride(ImageChromaSamplingMode.S444)
            .setUsePsnrQuantTable(true)
            .setUseInterlacing(true)
            .setCompressionLevel(5)
            .setWebpMethod(1)
            .setWebpImageHint(Configuration.ImageHint.DEFAULT)
            .build();
    testLoopback(configuration);
  }

  private void testLoopback(final Configuration original) {
    final Configuration twin = loopback(original);
    assertThat(twin).isEqualTo(original);
  }

  @DoNotStrip
  private native HybridData initHybrid();

  @DoNotStrip
  private native Configuration loopback(Configuration encodeRequirement);
}
