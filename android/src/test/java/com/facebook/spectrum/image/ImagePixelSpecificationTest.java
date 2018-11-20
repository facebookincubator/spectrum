/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import static org.fest.assertions.api.Assertions.assertThat;

import android.graphics.Bitmap;
import com.facebook.spectrum.image.ImagePixelSpecification.AlphaInfo;
import com.facebook.spectrum.image.ImagePixelSpecification.ColorModel;
import com.facebook.spectrum.image.ImagePixelSpecification.ComponentsOrder;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class ImagePixelSpecificationTest {

  @Test
  public void testPixelColorModel_whenMadeFromArgument_thenReturnsEnumInstance() {
    final ColorModel actual = ColorModel.from("rgb", 3, true);
    assertThat(actual).isSameAs(ColorModel.RGB);
  }

  @Test
  public void testImagePixelSpecification_whenMadeFromArgument_thenReturnsEnumInstance() {
    final ImagePixelSpecification actual =
        ImagePixelSpecification.from(ColorModel.RGB, 4, AlphaInfo.FIRST, ComponentsOrder.NATURAL);
    assertThat(actual).isSameAs(ImagePixelSpecification.ARGB);
  }

  @Test
  public void testImagePixelSpecification_whenFromBitmapArgb_thenReturnsRgba() {
    // Android's ARGB is actually RGBA
    assertThat(ImagePixelSpecification.from(Bitmap.Config.ARGB_8888))
        .isSameAs(ImagePixelSpecification.RGBA);
  }

  @Test
  public void testImagePixelSpecification_whenFromBitmapAlpha_thenReturnsGray() {
    // Android's ALPHA_8 is usually used for gray scale use cases
    assertThat(ImagePixelSpecification.from(Bitmap.Config.ALPHA_8))
        .isSameAs(ImagePixelSpecification.GRAY);
  }
}
