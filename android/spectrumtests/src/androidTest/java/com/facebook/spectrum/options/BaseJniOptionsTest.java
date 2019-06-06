/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.options;

import com.facebook.spectrum.Configuration;
import com.facebook.spectrum.image.EncodedImageFormat;
import com.facebook.spectrum.image.ImageMetadata;
import com.facebook.spectrum.image.ImagePixelSpecification;
import com.facebook.spectrum.image.ImageSize;
import com.facebook.spectrum.image.ImageSpecification;
import com.facebook.spectrum.requirements.EncodeRequirement;
import com.facebook.spectrum.requirements.ResizeRequirement;

/* package */ abstract class BaseJniOptionsTest {

  final Configuration TEST_CONFIGURATION = Configuration.makeEmpty();
  final ImageSpecification TEST_IMAGE_SPECIFICATION =
      new ImageSpecification(
          new ImageSize(640, 480), EncodedImageFormat.JPEG, ImagePixelSpecification.RGB);
  final int TEST_ROTATE_REQUIREMENT = 90;
  final ResizeRequirement TEST_RESIZE_REQUIREMENT =
      new ResizeRequirement(ResizeRequirement.Mode.EXACT, new ImageSize(480, 320));
  final ImageMetadata TEST_METADATA = new ImageMetadata();
  final EncodeRequirement TEST_ENCODE_REQUIREMENT =
      new EncodeRequirement(EncodedImageFormat.JPEG, 90, EncodeRequirement.Mode.LOSSY);
}
