/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.image;

import android.graphics.Bitmap;
import com.facebook.jni.annotations.DoNotStrip;
import javax.annotation.concurrent.Immutable;

/**
 * The specification explains how pixels and the color information is stored as a continuous stream
 * of bytes.
 */
@DoNotStrip
@Immutable
public enum ImagePixelSpecification {
  @DoNotStrip
  RGB(ColorModel.RGB, 3, AlphaInfo.NONE, ComponentsOrder.NATURAL),

  @DoNotStrip
  RGBA(ColorModel.RGB, 4, AlphaInfo.LAST, ComponentsOrder.NATURAL),

  @DoNotStrip
  GRAY(ColorModel.GRAY, 1, AlphaInfo.NONE, ComponentsOrder.NATURAL),

  @DoNotStrip
  GRAY_A(ColorModel.GRAY, 1, AlphaInfo.LAST, ComponentsOrder.NATURAL),

  @DoNotStrip
  A_GRAY(ColorModel.GRAY, 1, AlphaInfo.FIRST, ComponentsOrder.NATURAL),

  @DoNotStrip
  ARGB(ColorModel.RGB, 4, AlphaInfo.FIRST, ComponentsOrder.NATURAL),

  @DoNotStrip
  BGR(ColorModel.RGB, 3, AlphaInfo.NONE, ComponentsOrder.REVERSED),

  @DoNotStrip
  BGRA(ColorModel.RGB, 4, AlphaInfo.FIRST, ComponentsOrder.REVERSED),

  @DoNotStrip
  ABGR(ColorModel.RGB, 4, AlphaInfo.LAST, ComponentsOrder.REVERSED);

  /** Number of color components and how to interpret them */
  @DoNotStrip public final ColorModel colorModel;

  /** The number of bytes per pixel (covering all its components). */
  @DoNotStrip public final int bytesPerPixel;

  /** Information about the optional alpha channel */
  @DoNotStrip public final AlphaInfo alphaInfo;

  /** Order of the components in memory representation . E.g. BGR instead of RGB. */
  @DoNotStrip public final ComponentsOrder componentsOrder;

  ImagePixelSpecification(
      final ColorModel colorModel,
      final int bytesPerPixel,
      final AlphaInfo alphaInfo,
      final ComponentsOrder componentsOrder) {
    this.colorModel = colorModel;
    this.bytesPerPixel = bytesPerPixel;
    this.alphaInfo = alphaInfo;
    this.componentsOrder = componentsOrder;
  }

  @DoNotStrip
  public static ImagePixelSpecification from(final Bitmap.Config bitmapConfig) {
    switch (bitmapConfig) {
      case ALPHA_8:
        // Android's ALPHA_8 is used equivalent to a one-channel gray image
        return GRAY;
      case ARGB_8888:
        // Android's ARGB is actually RGBA when accessed natively
        return RGBA;
      default:
        throw new IllegalArgumentException("Unsupported Bitmap.Config: " + bitmapConfig);
    }
  }

  /** @return matching {@link ImagePixelSpecification} for the given values. Throws otherwise */
  @DoNotStrip
  static ImagePixelSpecification from(
      final ColorModel colorModel,
      final int bytesPerPixel,
      final AlphaInfo alphaInfo,
      final ComponentsOrder componentsOrder) {
    for (ImagePixelSpecification imagePixelSpecification : values()) {
      if (imagePixelSpecification.colorModel == colorModel
          && imagePixelSpecification.bytesPerPixel == bytesPerPixel
          && imagePixelSpecification.alphaInfo == alphaInfo
          && imagePixelSpecification.componentsOrder == componentsOrder) {
        return imagePixelSpecification;
      }
    }
    throw new IllegalArgumentException("Unsupported ImagePixelSpecification");
  }

  /** Order of the components in memory representation */
  @DoNotStrip
  @Immutable
  enum ComponentsOrder {
    @DoNotStrip
    NATURAL(0),

    @DoNotStrip
    REVERSED(1);

    @DoNotStrip private final int value;

    ComponentsOrder(final int value) {
      this.value = value;
    }

    /** @return matching {@link ComponentsOrder} for the given values. Throws otherwise */
    @DoNotStrip
    static ComponentsOrder from(final int value) {
      for (ComponentsOrder componentsOrder : ComponentsOrder.values()) {
        if (componentsOrder.value == value) {
          return componentsOrder;
        }
      }
      throw new IllegalArgumentException("Unsupported PixelComponentsOrder");
    }
  }

  /**
   * A color model defines the image's components and how they are interpreted to recreate the
   * intended color
   */
  @DoNotStrip
  @Immutable
  enum ColorModel {
    @DoNotStrip
    RGB("rgb", 3, true),

    @DoNotStrip
    GRAY("gray", 1, true),

    @DoNotStrip
    CMYK("cmyk", 4, false),

    @DoNotStrip
    YCBCR("ycbcr", 3, false),

    @DoNotStrip
    YCBCRK("ycbcrk", 4, false);

    /** Human readable identifier of the color model (e.g. `RGB`). */
    @DoNotStrip public final String identifier;

    /** Number of independent color components in the color model (e.g. 3 for for `RGB`) */
    @DoNotStrip public final int numberOfColorComponents;

    /**
     * True if the color model supports an alpha channel. For example, `RGB` supports an alpha
     * channel whereas `CMYK` does not.
     */
    @DoNotStrip public final boolean supportsExtraAlphaChannel;

    ColorModel(
        final String identifier,
        final int numberOfColorComponents,
        final boolean supportsExtraAlphaChannel) {
      this.identifier = identifier;
      this.numberOfColorComponents = numberOfColorComponents;
      this.supportsExtraAlphaChannel = supportsExtraAlphaChannel;
    }

    /** @return matching {@link ColorModel} for the given values. Throws otherwise */
    @DoNotStrip
    static ColorModel from(
        final String identifier,
        final int numberOfColorComponents,
        final boolean supportsExtraAlphaChannel) {
      for (ColorModel colorModel : ColorModel.values()) {
        if (colorModel.identifier.equals(identifier)
            && colorModel.numberOfColorComponents == numberOfColorComponents
            && colorModel.supportsExtraAlphaChannel == supportsExtraAlphaChannel) {
          return colorModel;
        }
      }
      throw new IllegalArgumentException("Unsupported PixelColorModel");
    }
  }
  /**
   * The alpha info specifies the behaviour of the optional alpha channel
   */
  @DoNotStrip
  @Immutable
  enum AlphaInfo {
    /** No alpha channel */
    @DoNotStrip
    NONE(0),

    /** Alpha channel comes before any other component */
    @DoNotStrip
    FIRST(1),

    /** Alpha channel comes after any other component */
    @DoNotStrip
    LAST(2),

    /** Alpha channel comes before any other component and must be ignored */
    @DoNotStrip
    SKIP_FIRST(3),

    /** Alpha channel comes after any other component and must be ignored */
    @DoNotStrip
    SKIP_LAST(4),

    /**
     * Alpha channel comes before any other component and the other components are premultiplied
     * with its value
     */
    @DoNotStrip
    PREMULTIPLIED_FIRST(5),

    /**
     * Alpha channel comes after any other component and the other compoenents are premultiplied
     * with its value
     */
    @DoNotStrip
    PREMULTIPLIED_LAST(6);

    @DoNotStrip private final int value;

    AlphaInfo(final int value) {
      this.value = value;
    }

    /** @return matching {@link AlphaInfo} for the given values. Throws otherwise */
    @DoNotStrip
    static AlphaInfo from(final int value) {
      for (AlphaInfo alphaInfo : AlphaInfo.values()) {
        if (alphaInfo.value == value) {
          return alphaInfo;
        }
      }
      throw new IllegalArgumentException("Unsupported PixelAlphaInfo");
    }
  }

  @Override
  public String toString() {
    return "ImagePixelSpecification{"
        + "colorModel="
        + colorModel
        + ", bytesPerPixel="
        + bytesPerPixel
        + ", alphaInfo="
        + alphaInfo
        + ", componentsOrder="
        + componentsOrder
        + '}';
  }
}
