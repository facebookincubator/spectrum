/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.requirements;

import static org.fest.assertions.api.Assertions.assertThat;

import android.graphics.Rect;
import android.graphics.RectF;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class CropRequirementTest {

  @Test
  public void testCropRequirement_whenAbsoluteConstructed_thenCorrectValuesAreSet() {
    final CropRequirement cropRequirement = CropRequirement.makeAbsoluteToOrigin(1, 2, 3, 4, true);
    assertAbsoluteCropping(cropRequirement);
  }

  @Test
  public void testCropRequirement_whenAbsoluteConstructedFromRect_thenCorrectValuesAreSet() {
    final Rect rect = new Rect(1, 2, 3, 4);
    final CropRequirement cropRequirement = CropRequirement.makeAbsoluteToOrigin(rect, true);
    assertAbsoluteCropping(cropRequirement);
  }

  private static void assertAbsoluteCropping(final CropRequirement cropRequirement) {
    assertThat(cropRequirement)
        .isExactlyInstanceOf(CropRequirement.CropAbsoluteToOriginRequirement.class);

    final CropRequirement.CropAbsoluteToOriginRequirement cropAbsoluteToOriginRequirement =
        (CropRequirement.CropAbsoluteToOriginRequirement) cropRequirement;

    assertThat(cropAbsoluteToOriginRequirement.left).isEqualTo(1);
    assertThat(cropAbsoluteToOriginRequirement.top).isEqualTo(2);
    assertThat(cropAbsoluteToOriginRequirement.right).isEqualTo(3);
    assertThat(cropAbsoluteToOriginRequirement.bottom).isEqualTo(4);

    assertThat(cropRequirement.mustBeExact).isTrue();
  }

  @Test
  public void testCropRequirement_whenRelativeConstructed_thenCorrectValuesAreSet() {
    final CropRequirement cropRequirement =
        CropRequirement.makeRelativeToOrigin(.1f, .2f, .3f, .4f, false);
    assertRelativeCropping(cropRequirement);
  }

  @Test
  public void testCropRequirement_whenRelativeConstructedFromRectF_thenCorrectValuesAreSet() {
    final RectF rect = new RectF(.1f, .2f, .3f, .4f);
    final CropRequirement cropRequirement = CropRequirement.makeRelativeToOrigin(rect, false);
    assertRelativeCropping(cropRequirement);
  }

  private static void assertRelativeCropping(final CropRequirement cropRequirement) {
    assertThat(cropRequirement)
        .isExactlyInstanceOf(CropRequirement.CropRelativeToOriginRequirement.class);

    final CropRequirement.CropRelativeToOriginRequirement cropRelativeToOriginRequirement =
        (CropRequirement.CropRelativeToOriginRequirement) cropRequirement;

    assertThat(cropRelativeToOriginRequirement.left).isEqualTo(.1f);
    assertThat(cropRelativeToOriginRequirement.top).isEqualTo(.2f);
    assertThat(cropRelativeToOriginRequirement.right).isEqualTo(.3f);
    assertThat(cropRelativeToOriginRequirement.bottom).isEqualTo(.4f);

    assertThat(cropRequirement.mustBeExact).isFalse();
  }
}
