// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample

import android.app.Application
import com.facebook.spectrum.SpectrumSoLoader

@Suppress("unused")
class SampleApplication : Application() {

    override fun onCreate() {
        super.onCreate()
        SpectrumSoLoader.init(this)

        // try this variant if the Facebook SoLoader creates troubles for your release distribution
        // SpectrumSoLoader.init(this, SpectrumSoLoader.SystemSoLoaderImpl())
    }
}
