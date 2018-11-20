/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.options;

import android.graphics.Bitmap;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.image.ImagePixelSpecification;
import com.facebook.spectrum.utils.Preconditions;
import javax.annotation.concurrent.Immutable;

/**
 * This Options object encapsulates all parameters for a transform operation. A transcode operation
 * reads a bitmap and creates a bitmap.
 */
@DoNotStrip
@Immutable
public class TransformOptions extends Options {

  private TransformOptions(final Builder builder) {
    super(builder);
  }

  @Override
  public String toString() {
    return toString("TransformOptions");
  }

  /**
   * Creates a default {@link TransformOptions.Builder} with the output pixel specification that
   * matches {@link Bitmap.Config#ARGB_8888}.
   */
  public static TransformOptions.Builder Builder() {
    return Builder(ImagePixelSpecification.from(Bitmap.Config.ARGB_8888));
  }

  /** Creates a {@link TransformOptions.Builder} with the given output pixel specification. */
  public static TransformOptions.Builder Builder(
      final ImagePixelSpecification outputPixelSpecification) {
    return new TransformOptions.Builder(outputPixelSpecification);
  }

  public static class Builder extends Options.Builder<TransformOptions.Builder> {

    /** Creates a {@link TransformOptions.Builder} with the given output pixel specification. */
    private Builder(final ImagePixelSpecification outputPixelSpecification) {
      super();
      outputPixelSpecification(outputPixelSpecification);
    }

    /**
     * If set, defines the pixel specification images should be converted to before being passed to
     * the compressor. An exception will be thrown if the pixel specification doesn't fit the image
     * format.
     */
    public Builder outputPixelSpecification(
        final ImagePixelSpecification outputPixelSpecification) {
      this.outputPixelSpecification = Preconditions.checkNotNull(outputPixelSpecification);
      return this;
    }

    @Override
    public TransformOptions build() {
      return new TransformOptions(this);
    }
  }
}
