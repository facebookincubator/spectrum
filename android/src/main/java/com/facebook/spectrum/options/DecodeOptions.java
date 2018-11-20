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
 * This Options object encapsulates all parameters for a decode operation. A decode operation reads
 * an encoded image and creates a bitmap.
 */
@DoNotStrip
@Immutable
public class DecodeOptions extends Options {

  private DecodeOptions(final Builder builder) {
    super(builder);
  }

  /**
   * Creates a default {@link DecodeOptions.Builder} with the output pixel specification that
   * matches {@link Bitmap.Config#ARGB_8888}.
   */
  public static Builder Builder() {
    return Builder(ImagePixelSpecification.from(Bitmap.Config.ARGB_8888));
  }

  /** Creates a {@link DecodeOptions.Builder} with the given output pixel specification. */
  public static Builder Builder(final ImagePixelSpecification outputPixelSpecification) {
    return new Builder(outputPixelSpecification);
  }

  @Override
  public String toString() {
    return toString("DecodeOptions");
  }

  /** See {@link DecodeOptions} */
  public static class Builder extends Options.Builder<DecodeOptions.Builder> {

    /** Creates a {@link DecodeOptions.Builder} with the given output pixel specification. */
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
    public DecodeOptions build() {
      return new DecodeOptions(this);
    }
  }
}
