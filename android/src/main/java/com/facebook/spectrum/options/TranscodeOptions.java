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
 * This Options object encapsulates all parameters for a transcode operation. A transcode operation
 * reads an encoded and creates an encoded image.
 */
@DoNotStrip
@Immutable
public class TranscodeOptions extends Options {

  private TranscodeOptions(final Builder builder) {
    super(builder);
  }

  @Override
  public String toString() {
    return toString("TranscodeOptions");
  }

  /** Creates an {@link TranscodeOptions.Builder} with the given {@link EncodeRequirement}. */
  public static TranscodeOptions.Builder Builder(final EncodeRequirement encodeRequirement) {
    return new TranscodeOptions.Builder(encodeRequirement);
  }

  public static class Builder extends Options.Builder<TranscodeOptions.Builder> {

    /** Creates an {@link TranscodeOptions.Builder} with the given {@link EncodeRequirement}. */
    private Builder(final EncodeRequirement encodeRequirement) {
      super();
      encodeRequirement(encodeRequirement);
    }

    /** Optional metadata to consider. */
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
    public TranscodeOptions build() {
      return new TranscodeOptions(this);
    }
  }
}
