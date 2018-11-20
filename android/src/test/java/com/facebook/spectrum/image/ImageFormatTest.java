/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import static org.fest.assertions.api.Assertions.assertThat;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class ImageFormatTest {

  @Test
  public void testImageFormat_whenComparingTwoEqualInstances_thenEquals() {
    final ImageFormat format = ImageFormat.BITMAP;
    final ImageFormat format2 = new ImageFormat("bitmap");

    assertThat(format).isEqualTo(format2);
  }
}
