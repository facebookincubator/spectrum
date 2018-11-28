/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import com.facebook.jni.annotations.DoNotStrip;
import javax.annotation.concurrent.Immutable;

/** Chroma sampling describe the number of luma information per chroma information. */
@DoNotStrip
@Immutable
public enum ImageChromaSamplingMode {
  /** Sampling mode 4:4:4 stores luma and chroma information at the same resolution */
  S444(0),

  /** Sampling mode 4:2:0 uses 4 luma information per chroma information */
  S420(1),

  /** Sampling mode 4:2:2 uses 2 horizontal luma information per chroma information */
  S422(2),

  /** Sampling mode 4:1:1 uses 4 horizontal luma information per chroma information */
  S411(3),

  /** Sampling mode 4:4:0 uses 2 vertical luma information per chroma information */
  S440(4);

  @DoNotStrip public final int value;

  @DoNotStrip
  ImageChromaSamplingMode(int value) {
    this.value = value;
  }

  @DoNotStrip
  static ImageChromaSamplingMode from(final int value) {
    for (final ImageChromaSamplingMode chromaSamplingMode : values()) {
      if (value == chromaSamplingMode.value) {
        return chromaSamplingMode;
      }
    }
    throw new IllegalArgumentException("Unsupported ChromaSamplingMode");
  }
}
