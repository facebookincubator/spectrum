/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.options;

import static org.fest.assertions.api.Assertions.assertThat;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class TranscodeOptionsTest extends BaseOptionsTest {

  @Test
  public void testSetters_whenEverythingSet_thenBuiltObjectConforms() {
    final TranscodeOptions options =
        TranscodeOptions.Builder(mockEncodeRequirement)
            .metadata(mockMetadata)
            .configuration(mockConfiguration)
            .crop(mockCropRequirement)
            .resize(mockResizeRequirement)
            .rotate(mockRotateRequirement)
            .build();

    assertThat(options.encodeRequirement).isEqualTo(mockEncodeRequirement);
    assertThat(options.metadata).isEqualTo(mockMetadata);
    assertThat(options.configuration).isEqualTo(mockConfiguration);
    assertThat(options.transformations.cropRequirement).isEqualTo(mockCropRequirement);
    assertThat(options.transformations.resizeRequirement).isEqualTo(mockResizeRequirement);
    assertThat(options.transformations.rotateRequirement).isEqualTo(mockRotateRequirement);
  }

  @Test
  public void testToString_whenToString_thenContainsDecodeOptions() {
    final TranscodeOptions options = TranscodeOptions.Builder(mockEncodeRequirement).build();
    assertThat(options.toString()).contains("TranscodeOptions");
  }
}
