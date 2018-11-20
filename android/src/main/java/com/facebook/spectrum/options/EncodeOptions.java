/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.options;

import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.image.ImageMetadata;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.utils.Preconditions;
import javax.annotation.concurrent.Immutable;

/**
 * This Options object encapsulates all parameters for an encode operation. An encode operation
 * reads a bitmap and creates an encoded image.
 */
@DoNotStrip
@Immutable
public class EncodeOptions extends Options {

  private EncodeOptions(final Builder builder) {
    super(builder);
  }

  @Override
  public String toString() {
    return toString("EncodeOptions");
  }

  /** Creates an {@link EncodeOptions.Builder} with the given {@link EncodeRequirement}. */
  public static EncodeOptions.Builder Builder(final EncodeRequirement encodeRequirement) {
    return new EncodeOptions.Builder(encodeRequirement);
  }

  public static class Builder extends Options.Builder<EncodeOptions.Builder> {

    /** Creates an {@link EncodeOptions.Builder} with the given {@link EncodeRequirement}. */
    private Builder(final EncodeRequirement encodeRequirement) {
      super();
      encodeRequirement(encodeRequirement);
    }

    /** Optional metadata to consider for the operation. */
    public Builder metadata(final ImageMetadata metadata) {
      this.metadata = Preconditions.checkNotNull(metadata);
      return this;
    }

    /** The {@link EncodeRequirement} to apply. Dictates the final image format. */
    protected Builder encodeRequirement(final EncodeRequirement encodeRequirement) {
      this.encodeRequirement = Preconditions.checkNotNull(encodeRequirement);
      return this;
    }

    @Override
    public EncodeOptions build() {
      return new EncodeOptions(this);
    }
  }
}
