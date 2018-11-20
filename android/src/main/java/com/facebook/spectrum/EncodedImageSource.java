/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import com.facebook.spectrum.utils.Preconditions;
import java.io.ByteArrayInputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

/** Abstraction for any encoded input */
public class EncodedImageSource implements Closeable {

  /** The input stream that is read from by Spectrum. */
  private final InputStream mInputStream;

  /**
   * Controls whether calling {@link #close()} on this object has an effect. It is true for all
   * creations where we control the creation of the input stream. When given an input stream that is
   * controlled outside, we don't close it.
   */
  private final boolean mCloseUnderlyingStream;

  private EncodedImageSource(final InputStream inputStream, final boolean closeUnderlyingStream) {
    mInputStream = Preconditions.checkNotNull(inputStream);
    mCloseUnderlyingStream = closeUnderlyingStream;
  }

  /**
   * Creates an {@link EncodedImageSource} for the given file.
   *
   * @param file The file to read from
   * @throws FileNotFoundException If the file does not exists
   */
  public static EncodedImageSource from(final File file) throws FileNotFoundException {
    return new EncodedImageSource(new FileInputStream(file), true);
  }

  /**
   * Creates an {@link EncodedImageSource} for the given file.
   *
   * @param filePath The file to read from
   * @throws FileNotFoundException If the file does not exists or is not readable
   */
  public static EncodedImageSource from(final String filePath) throws FileNotFoundException {
    return from(new File(filePath));
  }

  /**
   * Creates an {@link EncodedImageSource} for the given byte array.
   *
   * @param bytes Array containing the encoded image
   */
  public static EncodedImageSource from(final byte[] bytes) {
    return new EncodedImageSource(new ByteArrayInputStream(bytes), true);
  }

  /**
   * Creates an {@link EncodedImageSource} for the given {@link InputStream}.
   *
   * @param inputStream Input stream to read from; the caller keeps ownership of the {@link
   *     InputStream} and is responsible to properly close it.
   */
  public static EncodedImageSource from(final InputStream inputStream) {
    return new EncodedImageSource(inputStream, false);
  }

  /** Returns the underlying {@link InputStream} */
  public InputStream getInputStream() {
    return mInputStream;
  }

  /** Closes the underlying {@link InputStream} if the {@link EncodedImageSource} owns it. */
  @Override
  public void close() throws IOException {
    if (mCloseUnderlyingStream) {
      mInputStream.close();
    }
  }

  @Override
  public String toString() {
    return "ImageSource{" + "mInputStream=" + mInputStream + '}';
  }
}
