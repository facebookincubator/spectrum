/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import android.graphics.Bitmap;
import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.options.EncodeOptions;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.testutils.SpectrumAssertUtils.Builder;
import java.io.IOException;
import org.fest.assertions.data.Offset;
import org.junit.Test;

public class SpectrumAssertUtilsTest {

  @Test
  public void testBuilder_whenCreating_thenNothingThrows() throws IOException {
    final EncodeOptions options =
        EncodeOptions.Builder(new EncodeRequirement(EncodedImageFormat.JPEG, 50)).build();

    final Builder builder =
        Builder.withTestImage(TestData.JPEG.PATH_85x128_Q15)
            .encoding(options)
            .comparingAgainstTestFile(TestData.JPEG.PATH_85x128_Q15)
            .assertingOutputFormat(EncodedImageFormat.JPEG)
            .usingSsimTarget(1.0f)
            .usingSsimTolerance(Offset.offset(0.05f));

    builder.validate();
  }

  @Test(expected = IllegalArgumentException.class)
  public void testBuilder_whenRequestingBadConfig_thenValidateThrows() {
    final DecodeOptions options = DecodeOptions.Builder().build();

    final Builder builder =
        Builder.withTestImage(TestData.JPEG.PATH_85x128_Q15)
            .decoding(options)
            .comparingAgainst(Bitmap.createBitmap(10, 10, Bitmap.Config.ARGB_8888))
            .assertingOutputExifOrientation(2);

    builder.validate();
  }
}
