/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import static org.fest.assertions.api.Assertions.assertThat;

import com.facebook.spectrum.image.ImageChromaSamplingMode;
import com.facebook.spectrum.image.ImageColor;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class ConfigurationTest {

  @Test
  public void testConfiguration_whenConfigurationDefault_thenAllNull() {
    final Configuration configuration = Configuration.Builder().build();

    assertThat(configuration).isNotNull();
    assertThat(configuration.interpretMetadata).isNull();
    assertThat(configuration.samplingMethod).isNull();
    assertThat(configuration.propagateChromaSamplingModeFromSource).isNull();
    assertThat(configuration.useTrellis).isNull();
    assertThat(configuration.useProgressive).isNull();
    assertThat(configuration.useOptimizeScan).isNull();
    assertThat(configuration.useCompatibleDcScanOpt).isNull();
    assertThat(configuration.chromaSamplingModeOverride).isNull();
    assertThat(configuration.usePsnrQuantTable).isNull();
    assertThat(configuration.useInterlacing).isNull();
    assertThat(configuration.compressionLevel).isNull();
    assertThat(configuration.defaultBackgroundColor).isNull();
    assertThat(configuration.webpMethod).isNull();
    assertThat(configuration.webpImageHint).isNull();
  }

  @Test
  public void testConfiguration_whenConfigurationSpecified_thenStored() {
    final Configuration configuration =
        Configuration.Builder()
            .setDefaultBackgroundColor(new ImageColor(12, 34, 56))
            .setInterpretMetadata(true)
            .setSamplingMethod(Configuration.SamplingMethod.Bicubic)
            .setPropagateChromaSamplingModeFromSource(true)
            .setUseTrellis(true)
            .setUseProgressive(true)
            .setUseOptimizeScan(true)
            .setUseCompatibleDcScanOpt(true)
            .setChromaSamplingModeOverride(ImageChromaSamplingMode.S444)
            .setUsePsnrQuantTable(true)
            .setUseInterlacing(true)
            .setCompressionLevel(9)
            .setWebpMethod(1)
            .setWebpImageHint(Configuration.ImageHint.DEFAULT)
            .build();

    assertThat(configuration).isNotNull();
    assertThat(configuration.interpretMetadata).isTrue();
    assertThat(configuration.samplingMethod).isEqualTo(Configuration.SamplingMethod.Bicubic);
    assertThat(configuration.propagateChromaSamplingModeFromSource).isTrue();
    assertThat(configuration.useTrellis).isTrue();
    assertThat(configuration.useProgressive).isTrue();
    assertThat(configuration.useOptimizeScan).isTrue();
    assertThat(configuration.useCompatibleDcScanOpt).isTrue();
    assertThat(configuration.chromaSamplingModeOverride).isEqualTo(ImageChromaSamplingMode.S444);
    assertThat(configuration.usePsnrQuantTable).isTrue();
    assertThat(configuration.useInterlacing).isTrue();
    assertThat(configuration.compressionLevel).isEqualTo(9);
    assertThat(configuration.defaultBackgroundColor).isNotNull();
    assertThat(configuration.defaultBackgroundColor.red).isEqualTo(12);
    assertThat(configuration.defaultBackgroundColor.green).isEqualTo(34);
    assertThat(configuration.defaultBackgroundColor.blue).isEqualTo(56);
    assertThat(configuration.webpImageHint).isEqualTo(Configuration.ImageHint.DEFAULT);
    assertThat(configuration.webpMethod).isEqualTo(1);
  }

  @Test
  public void testConfiguration_whenConfigurationForGraphicalImages_thenGraphicalOptionsTrue() {
    final Configuration configuration = Configuration.makeForImageContainingGraphics();

    assertThat(configuration).isNotNull();
    assertThat(configuration.useTrellis).isTrue();
    assertThat(configuration.chromaSamplingModeOverride).isEqualTo(ImageChromaSamplingMode.S444);
    assertThat(configuration.usePsnrQuantTable).isTrue();
  }
}
