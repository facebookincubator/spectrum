// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample.model

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.widget.Toast
import com.facebook.spectrum.image.EncodedImageFormat
import java.io.IOException

enum class TranscodeViewModel {
    INSTANCE;

    val TAG = "Spectrum"
    var listener: ((TranscodeViewModel) -> Unit)? = null

    var inputType = IoType.FILE
        set(type) {
            field = type
            listener?.invoke(this)
        }

    var outputType = IoType.FILE
        set(type) {
            field = type
            listener?.invoke(this)
        }

    var outputFormat = EncodedImageFormat.JPEG
        set(outputFormat) {
            field = outputFormat
            listener?.invoke(this)
        }

    var encodeMode: EncodeMode = EncodeMode.ANY
        set(encodeMode) {
            field = encodeMode
            listener?.invoke(this)
        }

    var rotate = Rotate.NO_ROTATION
        set(rotate) {
            field = rotate
            listener?.invoke(this)
        }

    var resize = Resize.NO_RESIZE
        set(resize) {
            field = resize
            listener?.invoke(this)
        }

    var crop = Crop.NO_CROP
        set(crop) {
            field = crop
            listener?.invoke(this)
        }

    var quality = "90"
        set(quality) {
            field = quality
            listener?.invoke(this)
        }

    var inputImageUri: Uri? = null
        private set

    var inputImageBitmap: Bitmap? = null
        private set

    var sourceSummary = ""
        private set

    var transcodeState = TranscodeState.NOT_STARTED
        set(transcodeState) {
            field = transcodeState
            listener?.invoke(this)
        }

    var outputImageBitmap: Bitmap? = null
        internal set

    var outputImageBytes: ByteArray? = null
        internal set

    var outputError: String? = null
        internal set


    fun setInputImage(context: Context, uri: Uri) {
        inputImageUri = uri
        val inputStream = context.contentResolver.openInputStream(uri)
        inputStream.use {
                val inputSize = inputStream!!.available()
                inputImageBitmap = BitmapFactory.decodeStream(inputStream)
                sourceSummary = getImageInformationSummary(inputImageBitmap!!, inputSize)
                listener?.invoke(this)
        }
    }

    fun reset() {
        transcodeState = TranscodeState.NOT_STARTED
        outputError = ""
        outputImageBitmap = null
        outputImageBytes = null
        listener?.invoke(this)
    }

    private fun getImageInformationSummary(bitmap: Bitmap, length: Int): String {
        val bitrateBitsPerPixel = 8.0f * length.toFloat() / (bitmap.width * bitmap.height).toFloat()
        return StringBuilder().apply {
            append("size: ${bitmap.width}x${bitmap.height} - ")
            append(String.format("len: %.2f KiB - ", length / 1024f))
            append(String.format("rate: %.2f bits/px", bitrateBitsPerPixel))
        }.toString()
    }
}
