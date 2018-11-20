/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import com.facebook.spectrum.utils.Preconditions;
import java.io.Closeable;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

/** Abstraction for any encoded output */
public class EncodedImageSink implements Closeable {

  /** The input stream that is written to by Spectrum. */
  private final OutputStream mOutputStream;

  /**
   * Controls whether calling {@link #close()} on this object has an effect. It is true for all
   * creations where we control the creation of the output stream. When given an output stream that
   * is controlled outside, we don't close it.
   */
  private final boolean mCloseUnderlyingStream;

  private EncodedImageSink(final OutputStream outputStream, final boolean closeUnderlyingStream) {
    mOutputStream = Preconditions.checkNotNull(outputStream);
    mCloseUnderlyingStream = closeUnderlyingStream;
  }

  /**
   * Creates an {@link EncodedImageSink} for the given file.
   *
   * @param file The file to write to
   * @throws FileNotFoundException If the file cannot be created
   */
  public static EncodedImageSink from(final File file) throws FileNotFoundException {
    return new EncodedImageSink(new FileOutputStream(file), true);
  }

  /**
   * Creates an {@link EncodedImageSink} for the given file.
   *
   * @param filePath The file to write to
   * @throws FileNotFoundException If the file cannot be created
   */
  public static EncodedImageSink from(final String filePath) throws FileNotFoundException {
    return from(new File(filePath));
  }

  /**
   * Creates an {@link EncodedImageSink} for the given {@link OutputStream}.
   *
   * @param outputStream to write to; the caller keeps ownership of the {@link OutputStream} and is
   *     responsible to properly close it.
   */
  public static EncodedImageSink from(final OutputStream outputStream) {
    return new EncodedImageSink(outputStream, false);
  }

  /** Returns the underlying {@link OutputStream} */
  public OutputStream getOutputStream() {
    return mOutputStream;
  }

  /** Closes the underlying {@link OutputStream} if the {@link EncodedImageSink} owns it. */
  @Override
  public void close() throws IOException {
    if (mCloseUnderlyingStream) {
      mOutputStream.close();
    }
  }

  @Override
  public String toString() {
    return "ImageSink{" + "mOutputStream=" + mOutputStream + '}';
  }
}
