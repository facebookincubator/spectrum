/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import static org.fest.assertions.api.Assertions.assertThat;

import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.image.ImagePixelSpecification;
import com.facebook.spectrum.image.ImageSize;
import com.facebook.spectrum.image.ImageSpecification;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class SpectrumResultsTest {

  @Test
  public void testTranscodeOptions_whenConstructed_thenValuesSet() {
    final ImageSpecification inputImageSpecification =
        new ImageSpecification(
            new ImageSize(1000, 1000), EncodedImageFormat.PNG, ImagePixelSpecification.ARGB);
    final ImageSpecification outputImageSpecification =
        new ImageSpecification(
            new ImageSize(100, 100), EncodedImageFormat.JPEG, ImagePixelSpecification.RGB);

    final SpectrumResult result =
        new SpectrumResult("ruleName", inputImageSpecification, outputImageSpecification, 100, 200);

    assertThat(result.getRuleName()).isEqualTo("ruleName");
    assertThat(result.getInputImageSpecification()).isEqualTo(inputImageSpecification);
    assertThat(result.getOutputImageSpecification()).isEqualTo(outputImageSpecification);
    assertThat(result.getTotalBytesRead()).isEqualTo(100);
    assertThat(result.getTotalBytesWritten()).isEqualTo(200);
  }
}
