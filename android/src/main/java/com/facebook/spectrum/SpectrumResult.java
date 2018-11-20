/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.image.ImageSpecification;
import javax.annotation.Nullable;

/**
 * A Result encapsulates information about the operation (e.g. the determined image format and size)
 * as well as information about the applied rule.
 */
@DoNotStrip
public class SpectrumResult {

  @DoNotStrip @Nullable private final String ruleName;
  @DoNotStrip @Nullable private final ImageSpecification inputImageSpecification;
  @DoNotStrip @Nullable private final ImageSpecification outputImageSpecification;
  @DoNotStrip private final long totalBytesRead;
  @DoNotStrip private final long totalBytesWritten;

  @DoNotStrip
  public SpectrumResult(
      @Nullable String ruleName,
      @Nullable ImageSpecification inputImageSpecification,
      @Nullable ImageSpecification outputImageSpecification,
      long totalBytesRead,
      long totalBytesWritten) {
    this.ruleName = ruleName;
    this.inputImageSpecification = inputImageSpecification;
    this.outputImageSpecification = outputImageSpecification;
    this.totalBytesRead = totalBytesRead;
    this.totalBytesWritten = totalBytesWritten;
  }

  /** <code>True</code> iff a rule has been applied to fulfill the request */
  @DoNotStrip
  public boolean isSuccessful() {
    return ruleName != null;
  }

  /** Name of the rule which has performed the operation. */
  @Nullable
  @DoNotStrip
  public String getRuleName() {
    return ruleName;
  }

  /** Detected input image specification for the operation. */
  @Nullable
  @DoNotStrip
  public ImageSpecification getInputImageSpecification() {
    return inputImageSpecification;
  }

  /** Determined output image specification for the operation. */
  @Nullable
  @DoNotStrip
  public ImageSpecification getOutputImageSpecification() {
    return outputImageSpecification;
  }

  /** Total number of bytes that has been read from the underlying image source */
  @DoNotStrip
  public long getTotalBytesRead() {
    return totalBytesRead;
  }

  /** Total number of bytes that has been written to the image sink */
  @DoNotStrip
  public long getTotalBytesWritten() {
    return totalBytesWritten;
  }

  @Override
  public String toString() {
    return "SpectrumResult{"
        + "ruleName='"
        + ruleName
        + '\''
        + ", inputImageSpecification="
        + inputImageSpecification
        + ", outputImageSpecification="
        + outputImageSpecification
        + ", totalBytesRead="
        + totalBytesRead
        + ", totalBytesWritten="
        + totalBytesWritten
        + '}';
  }
}
