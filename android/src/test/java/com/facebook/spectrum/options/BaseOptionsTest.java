/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.options;

import static org.mockito.Mockito.mock;

import com.facebook.spectrum.Configuration;
import com.facebook.spectrum.image.ImageMetadata;
import com.facebook.spectrum.image.ImagePixelSpecification;
import com.facebook.spectrum.image.ImageSpecification;
import com.facebook.spectrum.requirements.CropRequirement;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.requirements.ResizeRequirement;
import com.facebook.spectrum.requirements.RotateRequirement;

/** Provides common mocked instances of parameter classes for the other options tests */
abstract class BaseOptionsTest {

  protected EncodeRequirement mockEncodeRequirement = mock(EncodeRequirement.class);
  protected ResizeRequirement mockResizeRequirement = mock(ResizeRequirement.class);
  protected CropRequirement mockCropRequirement = mock(CropRequirement.class);
  protected RotateRequirement mockRotateRequirement = mock(RotateRequirement.class);
  protected ImageMetadata mockMetadata = mock(ImageMetadata.class);
  protected Configuration mockConfiguration = mock(Configuration.class);
  protected ImageSpecification mockImageSpecification = mock(ImageSpecification.class);

  protected ImagePixelSpecification mockOutputPixelSpecification =
      ImagePixelSpecification.RGB; // enum types cannot be mocked
}
