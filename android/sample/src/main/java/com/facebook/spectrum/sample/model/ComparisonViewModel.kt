// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample.model

import android.graphics.Bitmap
import android.graphics.BitmapFactory

enum class ComparisonViewModel {
    INSTANCE;

    var outputBitmap: Bitmap? = null
    var inputBitmap: Bitmap? = null

    fun updateFromTranscodeModel(model: TranscodeViewModel) {
        inputBitmap = model.inputImageBitmap
        outputBitmap = model.outputImageBitmap
                ?: BitmapFactory.decodeByteArray(model.outputImageBytes, 0, model.outputImageBytes!!.size)
    }
}
