/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import com.facebook.jni.annotations.DoNotStrip;
import javax.annotation.concurrent.Immutable;

/**
 * Represents a compressed image format such as {@link EncodedImageFormat#JPEG}. See {@link
 * ImageFormat} for uncompressed image formats.
 */
@DoNotStrip
@Immutable
public class EncodedImageFormat extends ImageFormat {

  /** A JPEG encoded image, see: https://en.wikipedia.org/wiki/JPEG */
  public static final EncodedImageFormat JPEG = new EncodedImageFormat("jpeg");

  /** A PNG encoded image, see https://en.wikipedia.org/wiki/Portable_Network_Graphics */
  public static final EncodedImageFormat PNG = new EncodedImageFormat("png");

  /** A WebP encoded image, see https://en.wikipedia.org/wiki/WebP */
  public static final EncodedImageFormat WEBP = new EncodedImageFormat("webp");

  /** A GIF encoded image, see https://en.wikipedia.org/wiki/GIF */
  public static final EncodedImageFormat GIF = new EncodedImageFormat("gif");

  /** A HEIF encoded image, see https://en.wikipedia.org/wiki/High_Efficiency_Image_File_Format */
  public static final EncodedImageFormat HEIF = new EncodedImageFormat("heif");

  @DoNotStrip
  public EncodedImageFormat(final String identifier) {
    super(identifier);
  }

  @Override
  @DoNotStrip
  public boolean isEncoded() {
    return true;
  }
}
