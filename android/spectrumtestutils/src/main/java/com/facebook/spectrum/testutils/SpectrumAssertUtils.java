/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import static com.facebook.spectrum.testutils.ImageComparison.ImageComparisonMethod.SSIM;
import static org.fest.assertions.api.Assertions.assertThat;

import android.graphics.Bitmap;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.image.ImageFormat;
import com.facebook.spectrum.image.ImageSpecification;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.options.EncodeOptions;
import com.facebook.spectrum.options.TranscodeOptions;
import com.facebook.spectrum.testutils.ExecutingAssertion.ExecutionResult;
import com.facebook.spectrum.utils.Preconditions;
import java.io.IOException;
import javax.annotation.Nullable;
import org.fest.assertions.data.Offset;

public class SpectrumAssertUtils {

  public static void executeAndAssert(final Spectrum spectrum, final Builder builder)
      throws Exception {
    builder.validate(); // throws if ill-configured

    final ExecutionResult executionResult = builder.executingAssertion.execute(spectrum, builder);
    assertThat(executionResult.spectrumResult.isSuccessful()).isTrue();

    if (builder.assertOutputFormat != null) {
      final ImageSpecification outputImageSpecification =
          executionResult.spectrumResult.getOutputImageSpecification();

      assertThat(outputImageSpecification.format).isEqualTo(builder.assertOutputFormat);
    }

    if (builder.comparisionBitmap != null) {
      final float similarity =
          ImageComparison.compare(builder.comparisionBitmap, executionResult.outputBitmap, SSIM);

      assertThat(similarity).isEqualTo(builder.ssimTarget, builder.ssimTolerance);
    }
  }

  public static class Builder {
    final String inputPath;

    @Nullable Bitmap comparisionBitmap = null;
    Offset<Float> ssimTolerance = Offset.offset(0.05f);
    float ssimTarget = 1.0f;

    ExecutingAssertion executingAssertion = null;
    @Nullable ImageFormat assertOutputFormat;
    @Nullable Integer assertOutputExifOrientation;

    private Builder(final String inputPath) {
      this.inputPath = inputPath;
    }

    public static Builder withTestImage(final String testImagePath) {
      return new Builder(testImagePath);
    }

    public Builder transcoding(final TranscodeOptions options) {
      this.executingAssertion = new ExecutingAssertion.Transcode(options);
      return this;
    }

    public Builder decoding(final DecodeOptions options) {
      this.executingAssertion = new ExecutingAssertion.Decode(options);
      return this;
    }

    public Builder encoding(final EncodeOptions options) {
      this.executingAssertion = new ExecutingAssertion.Encode(options);
      return this;
    }

    public Builder comparingAgainst(final Bitmap bitmap) {
      this.comparisionBitmap = bitmap;
      return this;
    }

    public Builder comparingAgainstTestFile(final String testImagePath) throws IOException {
      this.comparisionBitmap = TestData.getBitmap(testImagePath);
      return this;
    }

    public Builder usingSsimTarget(final float ssimTarget) {
      Preconditions.checkArgument(ssimTarget >= 0f && ssimTarget <= 1f);
      this.ssimTarget = ssimTarget;
      return this;
    }

    public Builder usingSsimTolerance(final Offset<Float> ssimTolerance) {
      Preconditions.checkArgument(ssimTolerance.value >= 0f && ssimTolerance.value <= 1f);
      this.ssimTolerance = ssimTolerance;
      return this;
    }

    public Builder usingSsimTolerance(final float offset) {
      return usingSsimTolerance(Offset.offset(offset));
    }

    public Builder assertingOutputFormat(final ImageFormat imageFormat) {
      this.assertOutputFormat = imageFormat;
      return this;
    }

    public Builder assertingOutputExifOrientation(final int exifOrientation) {
      this.assertOutputExifOrientation = exifOrientation;
      return this;
    }

    void validate() {
      if (executingAssertion == null) {
        throw new IllegalArgumentException(
            "When using TestCaseUtil.Builder, "
                + "one must specify the intended operation using either "
                + ".transcoding(...), .decoding(...), or .encoding(...)");
      }
      if (assertOutputExifOrientation != null) {
        if (executingAssertion instanceof ExecutingAssertion.Decode) {
          throw new IllegalArgumentException(
              "The assertOutputExifOrientation(...) option can only be applied to encoding and "
                  + "transcoding operations.");
        }
      }
    }
  }
}
