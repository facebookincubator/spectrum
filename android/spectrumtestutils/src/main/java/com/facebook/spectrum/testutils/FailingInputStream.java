/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.testutils;

import java.io.IOException;
import java.io.InputStream;

/**
 * Input stream that will throw once a certain number of bytes have been read. The thrown exception
 * be a {@link IOException} with the message {@link #EXCEPTION_MESSAGE}.
 */
public class FailingInputStream extends InputStream {

  public static final String EXCEPTION_MESSAGE = "Flux compensator implosion";

  private final InputStream underlyingInputStream;
  private long bytesBeforeFail;

  public FailingInputStream(final InputStream underlyingInputStream, final long bytesBeforeFail) {
    this.underlyingInputStream = underlyingInputStream;
    this.bytesBeforeFail = bytesBeforeFail;
  }

  @Override
  public int read() throws IOException {
    if (bytesBeforeFail == 0) {
      throw new IOException(EXCEPTION_MESSAGE);
    }
    bytesBeforeFail--;
    return underlyingInputStream.read();
  }

  @Override
  public int available() throws IOException {
    return underlyingInputStream.available();
  }
}
