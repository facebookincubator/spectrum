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
import com.facebook.spectrum.testutils.TestSoLoader;
import org.junit.Before;
import org.junit.Test;

@SuppressLint("MissingNativeLoadLibrary")
public class JniCropRequirementTest {

  private HybridData mHybridData;

  @Before
  public void setUp() {
    TestSoLoader.init();
    TestSoLoader.loadLibrary("spectrumtest");
    assertThat((mHybridData = initHybrid()).isValid()).isTrue();
  }

  @Test
  public void test_whenAbsoluteRequirement_thenDescriptionMatches() {
    testDescription(
        CropRequirement.makeAbsoluteToOrigin(10, 11, 12, 13, true),
        "type:\"absoluteToOrigin\"",
        "left:10",
        "top:11",
        "right:12",
        "bottom:13",
        "mustBeExact:1");
  }

  @Test
  public void test_whenRelativeRequirement_thenDescriptionMatches() {
    testDescription(
        CropRequirement.makeRelativeToOrigin(.1f, .2f, .3f, .4f, false),
        "type:\"relativeToOrigin\"",
        "left:0.1",
        "top:0.2",
        "right:0.3",
        "bottom:0.4",
        "mustBeExact:0");
  }

  private void testDescription(final CropRequirement original, final String... expectedSubstrings) {
    final String description = describe(original);
    for (final String expectedSubstring : expectedSubstrings) {
      assertThat(description).contains(expectedSubstring);
    }
  }

  @DoNotStrip
  private native HybridData initHybrid();

  /**
   * As the native type of a requirement::Crop hides the actual values, we rely on its describing
   * string representation for this test. Hence, this class only tests the Java->C++ conversion.
   */
  @DoNotStrip
  private native String describe(final CropRequirement encodeRequirement);
}
