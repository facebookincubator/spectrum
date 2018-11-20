/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum;

import com.facebook.spectrum.plugins.SpectrumPlugin;
import com.facebook.spectrum.plugins.SpectrumPluginJpeg;
import com.facebook.spectrum.plugins.SpectrumPluginPng;
import com.facebook.spectrum.plugins.SpectrumPluginWebp;

public class DefaultPlugins {

  private DefaultPlugins() {};

  public static SpectrumPlugin[] get() {
    return new SpectrumPlugin[] {
      SpectrumPluginJpeg.get(), SpectrumPluginPng.get(), SpectrumPluginWebp.get()
    };
  }
}
