/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import android.graphics.Bitmap;
import android.util.Log;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.options.EncodeOptions;
import com.facebook.spectrum.options.TranscodeOptions;
import com.facebook.spectrum.options.TransformOptions;
import java.io.Closeable;
import java.io.IOException;
import javax.annotation.Nullable;

/**
 * An executable Spectrum task such as decoding or transcoding. It allows delaying the execution
 * and wrapping it within appropriate logging and re-try mechanisms.
 */
/*package*/ interface SpectrumTask {
  SpectrumResult execute(final SpectrumHybrid spectrumHybrid) throws SpectrumException;

  class Decode implements SpectrumTask {

    private final EncodedImageSource mSource;
    private final BitmapTarget mBitmapTarget;
    private final DecodeOptions mDecodeOptions;

    /*package*/ Decode(
        final EncodedImageSource source,
        final BitmapTarget bitmapTarget,
        final DecodeOptions decodeOptions) {
      mSource = source;
      mBitmapTarget = bitmapTarget;
      mDecodeOptions = decodeOptions;
    }

    @Override
    public SpectrumResult execute(final SpectrumHybrid spectrumHybrid) throws SpectrumException {
      try {
        return spectrumHybrid.decode(mSource.getInputStream(), mBitmapTarget, mDecodeOptions);
      } finally {
        Helper.closeQuietly(mSource);
      }
    }
  }

  class Encode implements SpectrumTask {

    private final Bitmap mBitmap;
    private final EncodedImageSink mSink;
    private final EncodeOptions mEncodeOptions;

    /*package*/ Encode(
        final Bitmap bitmap, final EncodedImageSink sink, final EncodeOptions encodeOptions) {
      mBitmap = bitmap;
      mSink = sink;
      mEncodeOptions = encodeOptions;
    }

    @Override
    public SpectrumResult execute(final SpectrumHybrid spectrumHybrid) throws SpectrumException {
      try {
        return spectrumHybrid.encode(mBitmap, mSink.getOutputStream(), mEncodeOptions);
      } finally {
        Helper.closeQuietly(mSink);
      }
    }
  }

  class Transcode implements SpectrumTask {

    private final EncodedImageSource mSource;
    private final EncodedImageSink mSink;
    private final TranscodeOptions mTranscodeOptions;

    /*package*/ Transcode(
        final EncodedImageSource source,
        final EncodedImageSink sink,
        final TranscodeOptions transcodeOptions) {
      mSource = source;
      mSink = sink;
      mTranscodeOptions = transcodeOptions;
    }

    @Override
    public SpectrumResult execute(final SpectrumHybrid spectrumHybrid) throws SpectrumException {
      try {
        return spectrumHybrid.transcode(
            mSource.getInputStream(), mSink.getOutputStream(), mTranscodeOptions);
      } finally {
        Helper.closeQuietly(mSource);
        Helper.closeQuietly(mSink);
      }
    }
  }

  class Transform implements SpectrumTask {

    private final Bitmap mBitmap;
    private final BitmapTarget mBitmapTarget;
    private final TransformOptions mTransformOptions;

    /*package*/ Transform(
        Bitmap bitmap, BitmapTarget bitmapTarget, TransformOptions transformOptions) {
      mBitmap = bitmap;
      mBitmapTarget = bitmapTarget;
      mTransformOptions = transformOptions;
    }

    @Override
    public SpectrumResult execute(final SpectrumHybrid spectrumHybrid) throws SpectrumException {
      return spectrumHybrid.transform(mBitmap, mBitmapTarget, mTransformOptions);
    }
  }

  class Helper {
    private Helper() {};

    static void closeQuietly(@Nullable final Closeable closeable) {
      if (closeable == null) {
        return;
      }
      try {
        closeable.close();
      } catch (IOException e) {
        Log.e("Spectrum", "Could not close stream", e);
      }
    }
  }
}
