/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.image.ImageFormat;
import com.facebook.spectrum.logging.SpectrumLogger;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.options.EncodeOptions;
import com.facebook.spectrum.options.Options;
import com.facebook.spectrum.options.TranscodeOptions;
import com.facebook.spectrum.options.TransformOptions;
import com.facebook.spectrum.plugins.SpectrumPlugin;
import com.facebook.spectrum.utils.Preconditions;

/**
 * The {@link Spectrum} class provides access to the {@link #decode}, {@link #encode}, {@link
 * #transcode}, and {@link #transform} operations.
 *
 * <p>Once created, a {@link Spectrum} object is thread-safe in respect to the executed operations.
 * If ised repeatedly, consider retaining an instance around in your application.
 */
public class Spectrum {

  private final SpectrumHybrid mSpectrumHybrid;
  private final SpectrumLogger mSpectrumLogger;

  /**
   * Creates a new {@link Spectrum} with the given {@link SpectrumLogger} and the given {@link
   * SpectrumPlugin}s. A default {@link Configuration} is used.
   */
  public static Spectrum make(final SpectrumLogger spectrumLogger, final SpectrumPlugin[] plugins) {
    return new Spectrum(spectrumLogger, Configuration.makeEmpty(), plugins);
  }

  /**
   * Creates a new {@link Spectrum} with the given {@link SpectrumLogger}, global {@link
   * Configuration}, and the given {@link SpectrumPlugin}s.
   */
  private Spectrum(
      final SpectrumLogger spectrumLogger,
      final Configuration configuration,
      final SpectrumPlugin[] plugins) {
    mSpectrumHybrid = new SpectrumHybrid(configuration, plugins);
    mSpectrumLogger = Preconditions.checkNotNull(spectrumLogger);
  }

  /**
   * Creates a new {@link Spectrum} with the given {@link SpectrumLogger}, global {@link
   * Configuration}, and the given {@link SpectrumPlugin}s.
   */
  public static Spectrum make(
      final SpectrumLogger spectrumLogger,
      final Configuration configuration,
      final SpectrumPlugin... plugins) {
    return new Spectrum(spectrumLogger, configuration, plugins);
  }

  /** @return true iff the native code loaded and initialized properly. */
  public boolean isAvailable() {
    return mSpectrumHybrid.isAvailable();
  }

  /**
   * A decode operation reads an encoded image and creates a bitmap.
   *
   * @param source The {@link EncodedImageSource} to read from.
   * @param bitmapTarget The {@link BitmapTarget} to create and retain the created bitmap.
   * @param decodeOptions The {@link DecodeOptions} for this operation.
   * @param callerContext The caller context that identifies this particular call site.
   * @return {@link SpectrumResult} containing information about the execution.
   * @throws SpectrumException for invalid input, missing support and runtime errors.
   */
  public SpectrumResult decode(
      final EncodedImageSource source,
      final BitmapTarget bitmapTarget,
      final DecodeOptions decodeOptions,
      final Object callerContext)
      throws SpectrumException {
    final SpectrumTask task = new SpectrumTask.Decode(source, bitmapTarget, decodeOptions);
    return internalExecute(task, decodeOptions, callerContext);
  }

  /**
   * An encode operation reads a bitmap and creates an encoded image.
   *
   * @param bitmap The {@link Bitmap} to read from.
   * @param sink The {@link EncodedImageSink} to write the encoded image to.
   * @param encodeOptions The {@link EncodeOptions} for this operation.
   * @param callerContext The caller context that identifies this particular call site.
   * @return {@link SpectrumResult} containing information about the execution.
   * @throws SpectrumException for invalid input, missing support and runtime errors.
   */
  public SpectrumResult encode(
      final Bitmap bitmap,
      final EncodedImageSink sink,
      final EncodeOptions encodeOptions,
      final Object callerContext)
      throws SpectrumException {
    final SpectrumTask task = new SpectrumTask.Encode(bitmap, sink, encodeOptions);
    return internalExecute(task, encodeOptions, callerContext);
  }

  /**
   * A transcode operation reads an encoded image and creates an encoded image.
   *
   * @param source The {@link EncodedImageSource} to read from.
   * @param sink The {@link EncodedImageSink} to write the encoded image to.
   * @param transcodeOptions The {@link TranscodeOptions} for this operation.
   * @param callerContext The caller context that identifies this particular call site.
   * @return {@link SpectrumResult} containing information about the execution.
   * @throws SpectrumException for invalid input, missing support and runtime errors.
   */
  public SpectrumResult transcode(
      final EncodedImageSource source,
      final EncodedImageSink sink,
      final TranscodeOptions transcodeOptions,
      final Object callerContext)
      throws SpectrumException {
    final SpectrumTask task = new SpectrumTask.Transcode(source, sink, transcodeOptions);
    return internalExecute(task, transcodeOptions, callerContext);
  }

  /**
   * A transform operation reads a bitmap and creates a bitmap.
   *
   * @param bitmap The {@link Bitmap} to read from
   * @param bitmapTarget The {@link BitmapTarget} to create and retain the created bitmap.
   * @param transformOptions The {@link TransformOptions} for this operation.
   * @param callerContext The caller context that identifies this particular call site.
   * @return {@link SpectrumResult} containing information about the execution.
   * @throws SpectrumException for invalid input, missing support and runtime errors.
   */
  public SpectrumResult transform(
      final Bitmap bitmap,
      final BitmapTarget bitmapTarget,
      final TransformOptions transformOptions,
      final Object callerContext)
      throws SpectrumException {
    final SpectrumTask task = new SpectrumTask.Transform(bitmap, bitmapTarget, transformOptions);
    return internalExecute(task, transformOptions, callerContext);
  }

  /**
   * This method indicates whether the given image format is supported by Spectrum using the default
   * plugin.
   */
  public boolean isImageFormatSupported(final ImageFormat imageFormat) {
    if (ImageFormat.BITMAP.equals(imageFormat)
        || EncodedImageFormat.JPEG.equals(imageFormat)
        || EncodedImageFormat.PNG.equals(imageFormat)
        || EncodedImageFormat.WEBP.equals(imageFormat)) {
      return true;
    }

    // TODO T24989397: Add GIF support to Spectrum
    // TODO T32216944: Add HEIF support to Spectrum
    return false;
  }

  @SuppressLint("CatchGeneralException")
  private SpectrumResult internalExecute(
      final SpectrumTask spectrumTask, final Options options, final Object callerContext)
      throws SpectrumException {
    Preconditions.checkNotNull(spectrumTask);

    final Object loggingContext = mSpectrumLogger.onStart(options, callerContext);
    SpectrumResult result = null;
    try {
      return result = spectrumTask.execute(mSpectrumHybrid);
    } catch (final SpectrumException spectrumException) {
      mSpectrumLogger.onError(loggingContext, spectrumException);
      throw spectrumException;
    } catch (final Exception generalException) {
      mSpectrumLogger.onError(loggingContext, generalException);
      throw new SpectrumException(generalException);
    } finally {
      mSpectrumLogger.onFinish(loggingContext, result);
    }
  }
}
