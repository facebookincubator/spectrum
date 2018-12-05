/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import android.graphics.Bitmap;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.utils.Preconditions;
import javax.annotation.Nullable;
import javax.annotation.concurrent.Immutable;

/**
 * An image's specification contains all derived and extracted information besides the pixel
 * content.
 */
@DoNotStrip
@Immutable
public class ImageSpecification {

  @DoNotStrip public final ImageSize size;
  @DoNotStrip public final ImageFormat format;
  @DoNotStrip public final ImagePixelSpecification pixelSpecification;
  @DoNotStrip public final ImageOrientation orientation;
  @DoNotStrip @Nullable public final ImageChromaSamplingMode chromaSamplingMode;
  @DoNotStrip public final ImageMetadata metadata;

  /**
   * Constructor for an image specification with default orientation, no sampling and empty
   * metadata.
   */
  @DoNotStrip
  public ImageSpecification(
      final ImageSize size,
      final ImageFormat format,
      final ImagePixelSpecification pixelSpecification) {
    this(size, format, pixelSpecification, ImageOrientation.UP, null, ImageMetadata.empty());
  }

  @DoNotStrip
  public ImageSpecification(
      final ImageSize size,
      final ImageFormat format,
      final ImagePixelSpecification pixelSpecification,
      final ImageOrientation orientation,
      final @Nullable ImageChromaSamplingMode chromaSamplingMode,
      final ImageMetadata metadata) {
    this.size = Preconditions.checkNotNull(size);
    this.format = Preconditions.checkNotNull(format);
    this.pixelSpecification = Preconditions.checkNotNull(pixelSpecification);
    this.orientation = Preconditions.checkNotNull(orientation);
    this.chromaSamplingMode = chromaSamplingMode;
    this.metadata = metadata;
  }

  /**
   * Creates an image specification from the provided {@link Bitmap} object. In particular, the
   * pixel specification will be derived from the {@link Bitmap.Config}.
   */
  public static ImageSpecification fromBitmap(final Bitmap bitmap) {
    Preconditions.checkNotNull(bitmap);
    return new ImageSpecification(
        new ImageSize(bitmap.getWidth(), bitmap.getHeight()),
        ImageFormat.BITMAP,
        ImagePixelSpecification.from(bitmap.getConfig()));
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;

    final ImageSpecification that = (ImageSpecification) o;

    if (!size.equals(that.size)) return false;
    if (!format.equals(that.format)) return false;
    if (pixelSpecification != that.pixelSpecification) return false;
    if (orientation != that.orientation) return false;
    if (chromaSamplingMode != that.chromaSamplingMode) return false;
    return metadata != null ? metadata.equals(that.metadata) : that.metadata == null;
  }

  @Override
  public int hashCode() {
    int result = size.hashCode();
    result = 31 * result + format.hashCode();
    result = 31 * result + pixelSpecification.hashCode();
    result = 31 * result + orientation.hashCode();
    result = 31 * result + (chromaSamplingMode != null ? chromaSamplingMode.hashCode() : 0);
    result = 31 * result + (metadata != null ? metadata.hashCode() : 0);
    return result;
  }

  @Override
  public String toString() {
    return "ImageSpecification{"
        + "size="
        + size
        + ", format="
        + format
        + ", pixelSpecification="
        + pixelSpecification
        + ", orientation="
        + orientation
        + ", chromaSamplingMode="
        + chromaSamplingMode
        + ", metadata="
        + metadata
        + '}';
  }
}
