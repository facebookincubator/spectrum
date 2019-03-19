/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import android.graphics.Bitmap;
import com.facebook.spectrum.image.ImageFormat;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.options.EncodeOptions;
import com.facebook.spectrum.options.TranscodeOptions;
import com.facebook.spectrum.options.TransformOptions;

/**
 * The {@link ISpectrum} interface allows developers to create wrapping implementations of the
 * default {@link Spectrum} class (e.g. using lazy loading of native libraries).
 */
public interface ISpectrum {

  /** @return true iff the native code loaded and initialized properly. */
  boolean isAvailable();

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
  SpectrumResult decode(
      EncodedImageSource source,
      BitmapTarget bitmapTarget,
      DecodeOptions decodeOptions,
      Object callerContext)
      throws SpectrumException;

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
  SpectrumResult encode(
      Bitmap bitmap, EncodedImageSink sink, EncodeOptions encodeOptions, Object callerContext)
      throws SpectrumException;

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
  SpectrumResult transcode(
      EncodedImageSource source,
      EncodedImageSink sink,
      TranscodeOptions transcodeOptions,
      Object callerContext)
      throws SpectrumException;

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
  SpectrumResult transform(
      Bitmap bitmap,
      BitmapTarget bitmapTarget,
      TransformOptions transformOptions,
      Object callerContext)
      throws SpectrumException;

  /**
   * This method indicates whether the given image format is supported by Spectrum using the default
   * plugin.
   */
  boolean isImageFormatSupported(ImageFormat imageFormat);
}
