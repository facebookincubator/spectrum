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

/**
 * The interface to be implemented by the global logger for Spectrum events which are emitted for
 * all requests. The implementation needs to be thread-safe as Spectrum might execute multiple
 * operations in parallel.
 */
public interface SpectrumLogger {

  /**
   * Called before the operation is started.
   *
   * @param options The given {@link Options} for the operation
   * @param callerContext An integration specific object that provides information about the calling
   *     site
   * @return A logging context that will be returned to this class when the {@link #onFinish} and
   *     {@link #onError} methods are called. This can be used to differentiate operations. Can be
   *     null.
   */
  @Nullable
  Object onStart(final Options options, final Object callerContext);

  /**
   * Called for any exception that occurs during the operation.
   *
   * @param loggingContext The logging context that was returned initially by the {@link #onStart}
   *     method (if any).
   * @param exception The exception that was thrown
   */
  void onError(@Nullable final Object loggingContext, final Exception exception);

  /**
   * Called when the operation finishes. This method is guaranteed to be called exactly once and no
   * other callback is called afterwards for a given operation.
   *
   * @param loggingContext The logging context that was returned initially by the {@link #onStart}
   *     method (if any).
   * @param result The {@link SpectrumResult} returned by the operation
   */
  void onFinish(@Nullable final Object loggingContext, @Nullable final SpectrumResult result);
}
