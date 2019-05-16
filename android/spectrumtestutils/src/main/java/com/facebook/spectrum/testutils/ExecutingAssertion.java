/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import static org.fest.assertions.api.Assertions.assertThat;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import androidx.exifinterface.media.ExifInterface;
import com.facebook.spectrum.BitmapTarget;
import com.facebook.spectrum.EncodedImageSink;
import com.facebook.spectrum.EncodedImageSource;
import com.facebook.spectrum.Spectrum;
import com.facebook.spectrum.SpectrumResult;
import com.facebook.spectrum.options.DecodeOptions;
import com.facebook.spectrum.options.EncodeOptions;
import com.facebook.spectrum.options.TranscodeOptions;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;

public interface ExecutingAssertion {
  String CALLER_CONTEXT = "ExecutingAssertion";

  ExecutionResult execute(final Spectrum spectrum, final SpectrumAssertUtils.Builder builder)
      throws Exception;

  class Transcode implements ExecutingAssertion {
    private final TranscodeOptions options;

    /*package*/ Transcode(final TranscodeOptions options) {
      this.options = options;
    }

    @Override
    public ExecutionResult execute(
        final Spectrum spectrum, final SpectrumAssertUtils.Builder builder) throws Exception {
      final EncodedImageSource source =
          EncodedImageSource.from(TestData.getInputStream(builder.inputPath));
      final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
      final EncodedImageSink sink = EncodedImageSink.from(outputStream);

      final SpectrumResult spectrumResult =
          spectrum.transcode(source, sink, options, CALLER_CONTEXT);

      if (builder.assertOutputExifOrientation != null) {
        Utils.assertExifOrientation(builder, outputStream);
      }

      return new ExecutionResult(spectrumResult, Utils.byteArrayStreamToBitmap(outputStream));
    }
  }

  class Decode implements ExecutingAssertion {
    private final DecodeOptions options;

    /*package*/ Decode(final DecodeOptions options) {
      this.options = options;
    }

    @Override
    public ExecutionResult execute(
        final Spectrum spectrum, final SpectrumAssertUtils.Builder builder) throws Exception {
      final EncodedImageSource source =
          EncodedImageSource.from(TestData.getInputStream(builder.inputPath));
      final BitmapTarget bitmapTarget = new BitmapTarget();

      final SpectrumResult spectrumResult =
          spectrum.decode(source, bitmapTarget, options, CALLER_CONTEXT);

      assertThat(bitmapTarget.getBitmap()).isNotNull();

      return new ExecutionResult(spectrumResult, bitmapTarget.getBitmap());
    }
  }

  class Encode implements ExecutingAssertion {
    private final EncodeOptions options;

    /*package*/ Encode(final EncodeOptions options) {
      this.options = options;
    }

    @Override
    public ExecutionResult execute(
        final Spectrum spectrum, final SpectrumAssertUtils.Builder builder) throws Exception {
      final Bitmap inputBitmap = TestData.getBitmap(builder.inputPath);
      final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
      final EncodedImageSink sink = EncodedImageSink.from(outputStream);

      final SpectrumResult spectrumResult =
          spectrum.encode(inputBitmap, sink, options, CALLER_CONTEXT);

      if (builder.assertOutputExifOrientation != null) {
        Utils.assertExifOrientation(builder, outputStream);
      }

      return new ExecutionResult(spectrumResult, Utils.byteArrayStreamToBitmap(outputStream));
    }
  }

  class ExecutionResult {
    final SpectrumResult spectrumResult;
    final Bitmap outputBitmap;

    ExecutionResult(final SpectrumResult spectrumResult, final Bitmap outputBitmap) {
      this.spectrumResult = spectrumResult;
      this.outputBitmap = outputBitmap;
    }
  }

  class Utils {
    private static Bitmap byteArrayStreamToBitmap(final ByteArrayOutputStream outputStream) {
      final byte[] outputData = outputStream.toByteArray();
      return BitmapFactory.decodeByteArray(outputData, 0, outputData.length);
    }

    private static void assertExifOrientation(
        final SpectrumAssertUtils.Builder builder, final ByteArrayOutputStream outputStream)
        throws IOException {
      final ByteArrayInputStream byteArrayInputStream =
          new ByteArrayInputStream(outputStream.toByteArray());
      final ExifInterface exifInterface = new ExifInterface(byteArrayInputStream);

      final int actualOrientation = exifInterface.getAttributeInt(ExifInterface.TAG_ORIENTATION, 0);

      assertThat(actualOrientation).isEqualTo(builder.assertOutputExifOrientation);
    }
  }
}
