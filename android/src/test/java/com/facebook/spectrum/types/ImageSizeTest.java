/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.types;

import static org.fest.assertions.api.Assertions.assertThat;

import com.facebook.spectrum.image.ImageSize;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class ImageSizeTest {

  @Test
  public void testImageSize_whenBoundsSet_thenStoredCorrectly() {
    ImageSize imageSize = new ImageSize(640, 480);
    assertThat(imageSize.width).isEqualTo(640);
    assertThat(imageSize.height).isEqualTo(480);
  }

  @Test(expected = IllegalArgumentException.class)
  public void testImageSize_whenWidthNegative_thenThrows() {
    new ImageSize(-1, 480);
  }

  @Test(expected = IllegalArgumentException.class)
  public void testImageSize_whenHeightNegative_thenThrows() {
    new ImageSize(640, -1);
  }

  @Test(expected = IllegalArgumentException.class)
  public void testImageSize_whenWidthTooLarge_thenThrows() {
    new ImageSize(65537, 480);
  }

  @Test(expected = IllegalArgumentException.class)
  public void testImageSize_whenHeightTooLarge_thenThrows() {
    new ImageSize(640, 65537);
  }

  @Test
  public void testImageSize_whenToString_thenStringContainsAllInfo() {
    final ImageSize imageSize = new ImageSize(640, 480);
    final String toString = imageSize.toString();

    assertThat(toString).containsIgnoringCase("width=640");
    assertThat(toString).containsIgnoringCase("height=480");
  }
}
