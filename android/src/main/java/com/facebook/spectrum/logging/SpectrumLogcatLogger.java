/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.logging;

import android.util.Log;
import com.facebook.spectrum.SpectrumResult;
import com.facebook.spectrum.options.Options;
import com.facebook.spectrum.utils.Preconditions;
import java.util.concurrent.atomic.AtomicInteger;
import javax.annotation.Nullable;

/**
 * Simple implementation of {@link SpectrumLogger} which logs to Android's main log using {@link
 * Log}.
 *
 * <p>Use `adb logcat -s SpectrumLogger` on your developer machine to see the output.
 */
public class SpectrumLogcatLogger extends BaseSpectrumLogger {

  private static final String TAG = "SpectrumLogger";
  private static final AtomicInteger REQUEST_ID_PROVIDER = new AtomicInteger(1);

  private final int mLogLevel;
  private final int mErrorLogLevel;

  /**
   * Creates a new LogcatLogger that informative messages logs at level {@link Log#DEBUG} and error
   * messages at level {@link Log#WARN}.
   */
  public SpectrumLogcatLogger() {
    this(Log.DEBUG);
  }

  /**
   * Creates a new LogcatLogger with the given log level for informational messages. Error messages
   * will be logged at level {@link Log#WARN}.
   *
   * @param logLevel Informative log level as defined in {@link Log}
   */
  public SpectrumLogcatLogger(final int logLevel) {
    this(logLevel, Log.WARN);
  }

  /**
   * Creates a new LogcatLogger with the given log and error level
   *
   * @param logLevel Informative log level as defined in {@link Log}
   * @param logLevel Error log level as defined in {@link Log}
   */
  public SpectrumLogcatLogger(final int logLevel, final int errorLogLevel) {
    Preconditions.checkArgument(isValidLogLevel(logLevel));
    Preconditions.checkArgument(isValidLogLevel(errorLogLevel));
    mLogLevel = logLevel;
    mErrorLogLevel = errorLogLevel;
  }

  @Nullable
  @Override
  public Object onStart(final Options options, final Object callerContext) {
    final int requestId = REQUEST_ID_PROVIDER.getAndIncrement();
    Log.println(mLogLevel, TAG, "requestid:" + requestId + ", options:" + options);
    Log.println(mLogLevel, TAG, "requestid:" + requestId + ", callerContext: " + callerContext);
    return requestId;
  }

  @Override
  public void onError(@Nullable final Object loggingContext, final Exception exception) {
    final int requestId = (Integer) Preconditions.checkNotNull(loggingContext);
    Log.println(
        mErrorLogLevel,
        TAG,
        "requestid:"
            + requestId
            + ", error:"
            + exception.getMessage()
            + '\n'
            + Log.getStackTraceString(exception));
  }

  @Override
  public void onFinish(
      @Nullable final Object loggingContext, @Nullable final SpectrumResult result) {
    final int requestId = (Integer) Preconditions.checkNotNull(loggingContext);
    Log.println(mLogLevel, TAG, "requestid:" + requestId + ", result: " + result);
  }

  private static boolean isValidLogLevel(final int level) {
    return level >= Log.VERBOSE && level <= Log.ASSERT;
  }
}
