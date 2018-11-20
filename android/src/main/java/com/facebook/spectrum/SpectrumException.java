/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import com.facebook.jni.annotations.DoNotStrip;
import javax.annotation.Nullable;
import javax.annotation.concurrent.Immutable;

/** Thrown when the request was unable to be fulfilled by Spectrum. */
@Immutable
public class SpectrumException extends Exception {

  /** Name that groups similar exceptions together (e.g. all compressor failures) */
  @Nullable private final String name;

  /** The location of the error including method and line number */
  @Nullable private final String location;

  /** Additional verbose descriptions (e.g. the functions parameters and state) */
  @Nullable private final String description;

  @DoNotStrip
  private SpectrumException(
      @Nullable final String name,
      @Nullable final String message,
      @Nullable final String location,
      @Nullable final String description,
      @Nullable final Exception cause) {
    super(message, cause);
    this.name = name;
    this.location = location;
    this.description = description;
  }

  @DoNotStrip
  private SpectrumException(
      @Nullable final String name,
      @Nullable final String message,
      @Nullable final String location,
      @Nullable final String description) {
    this(name, message, location, description, null);
  }

  /** Creates new exception with the given message */
  public SpectrumException(final String message) {
    this(message, null);
  }

  /** Creates new exception with the given cause */
  public SpectrumException(final Exception cause) {
    this(null, cause);
  }

  /** Creates new exception with the given message and cause */
  public SpectrumException(final String message, final Exception cause) {
    this(message, message, null, null, cause);
  }

  /** Name that groups similar exceptions together (e.g. all compressor failures) */
  @Nullable
  public String getName() {
    return name;
  }

  /** The specific error message (e.g. input forms no valid JPEG stream) */
  @Override
  public String getMessage() {
    return super.getMessage();
  }

  /** The location of the error including method and line number */
  @Nullable
  public String getLocation() {
    return location;
  }

  /** Additional verbose descriptions (e.g. the functions parameters and state) */
  @Nullable
  public String getDescription() {
    return description;
  }

  @Override
  public String toString() {
    return "SpectrumException{"
        + "name='"
        + name
        + '\''
        + ", message='"
        + getMessage()
        + '\''
        + ", location='"
        + location
        + '\''
        + ", description='"
        + description
        + '\''
        + '}';
  }
}
