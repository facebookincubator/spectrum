/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.plugins;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.image.ImageFormat;
import com.facebook.spectrum.image.ImagePixelSpecification;
import com.facebook.spectrum.image.ImageSize;
import com.facebook.spectrum.image.ImageSpecification;
import com.facebook.spectrum.utils.Preconditions;

/**
 * Decompressor implementation using the system's {@link BitmapFactory}. This allows to support
 * decoding e.g GIF and HEIC files on Android versions that support this without extra Spectrum
 * plugins.
 *
 * <p>The supported (depending on the Android version) image mimetypes are listed in {@link
 * SpectrumPlatformDecompressor.ImageMimetype}.
 */
@DoNotStrip
public class SpectrumPlatformDecompressor {

  private static final Bitmap.Config BITMAP_CONFIG = Bitmap.Config.ARGB_8888;

  /** Mapping of mimetypes to {@link EncodedImageFormat} */
  public enum ImageMimetype {
    GIF("image/gif", EncodedImageFormat.GIF),
    HEIC("image/heic", EncodedImageFormat.HEIF),
    HEIF("image/heif", EncodedImageFormat.HEIF),
    JPEG("image/jpeg", EncodedImageFormat.JPEG),
    PNG("image/png", EncodedImageFormat.PNG),
    WEBP("image/webp", EncodedImageFormat.WEBP);

    private final String mimetype;
    private final EncodedImageFormat encodedImageFormat;

    ImageMimetype(final String mimetype, final EncodedImageFormat encodedImageFormat) {
      this.mimetype = mimetype;
      this.encodedImageFormat = encodedImageFormat;
    }
  }

  private final byte[] content;

  @DoNotStrip
  public SpectrumPlatformDecompressor(final byte[] content) {
    if (content == null || content.length == 0) {
      throw new IllegalStateException("content empty");
    }
    this.content = content;
  }

  @DoNotStrip
  public ImageSpecification getImageSpecification() {
    final BitmapFactory.Options options = new BitmapFactory.Options();
    options.inPreferredConfig = BITMAP_CONFIG;
    options.inJustDecodeBounds = true;

    BitmapFactory.decodeByteArray(content, 0, content.length, options);

    return new ImageSpecification(
        optionsToImageSize(options),
        optionsToFormat(options),
        optionsToPixelSpecification(options));
  }

  @DoNotStrip
  public Bitmap readBitmap() {
    final BitmapFactory.Options options = new BitmapFactory.Options();
    options.inPreferredConfig = BITMAP_CONFIG;

    Bitmap bitmap = BitmapFactory.decodeByteArray(content, 0, content.length, options);

    if (bitmap.getConfig() == null) {
      // HACK: Fixing issue with GIF on Android 7 API 24 where the pixel data is just the number of
      // pixels and not pixels*4 (ARGB_8888).
      // @see
      // https://android.googlesource.com/platform/frameworks/base/+/android-9.0.0_r8/graphics/java/android/graphics/Bitmap.java#833
      bitmap = bitmap.copy(BITMAP_CONFIG, false);
    }

    return bitmap;
  }

  private static ImageSize optionsToImageSize(final BitmapFactory.Options options) {
    return new ImageSize(options.outWidth, options.outHeight);
  }

  private static ImageFormat optionsToFormat(final BitmapFactory.Options options) {
    final String mimeType = Preconditions.checkNotNull(options.outMimeType);
    for (final ImageMimetype imageMimetype : ImageMimetype.values()) {
      if (mimeType.equalsIgnoreCase(imageMimetype.mimetype)) {
        return imageMimetype.encodedImageFormat;
      }
    }
    throw new IllegalArgumentException("Mimetype not handled: " + mimeType);
  }

  private static ImagePixelSpecification optionsToPixelSpecification(
      @SuppressWarnings("unused") final BitmapFactory.Options options) {
    return ImagePixelSpecification.from(BITMAP_CONFIG);
  }
}
