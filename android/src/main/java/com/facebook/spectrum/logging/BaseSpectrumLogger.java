/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.logging;

import com.facebook.spectrum.SpectrumResult;
import com.facebook.spectrum.options.Options;
import javax.annotation.Nullable;

/** Logger implementation that performs no operations */
public class BaseSpectrumLogger implements SpectrumLogger {

  @Nullable
  @Override
  public Object onStart(Options options, Object callerContext) {
    return null;
  }

  @Override
  public void onError(@Nullable Object loggingContext, Exception exception) {}

  @Override
  public void onFinish(@Nullable Object loggingContext, @Nullable SpectrumResult result) {}
}
