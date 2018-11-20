/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import android.graphics.Bitmap;
import com.facebook.jni.annotations.DoNotStrip;
import com.facebook.spectrum.utils.Preconditions;
import javax.annotation.Nullable;

/**
 * For decode and transform operations, this {@link BitmapTarget} is provided to the {@link
 * Spectrum#decode} or {@link Spectrum#transform} methods.
 *
 * <p>The {@link BitmapTarget} is an object that will allocate an Android {@link Bitmap} when asked
 * by the underlying implementation. Can be overridden to cater for custom Bitmap allocation
 * implementation.
 */
@DoNotStrip
public class BitmapTarget {

  @DoNotStrip @Nullable private Bitmap mBitmap;

  /**
   * Allocates a bitmap with the given width, height, and {@link Bitmap.Config#ARGB_8888}.
   *
   * @return True iff the allocation was successful.
   * @throws IllegalStateException if the target already has an allocated {@link Bitmap}.
   */
  @DoNotStrip
  public boolean createBitmapArgb(final int width, final int height) {
    Preconditions.checkState(mBitmap == null);
    mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
    return true;
  }

  /**
   * @return A previously allocated {@link Bitmap} created by {@link #createBitmapArgb}. Returns
   *     <code>null</code> if there has been no call to {@link #createBitmapArgb}.
   */
  @DoNotStrip
  @Nullable
  public Bitmap getBitmap() {
    return mBitmap;
  }
}
