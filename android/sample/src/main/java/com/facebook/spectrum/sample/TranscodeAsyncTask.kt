// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample

import android.content.Context
import android.graphics.Bitmap
import android.os.AsyncTask
import android.text.TextUtils
import android.util.Log
import com.facebook.spectrum.*
import com.facebook.spectrum.options.*
import com.facebook.spectrum.requirements.EncodeRequirement
import com.facebook.spectrum.sample.model.ConfigurationViewModel
import com.facebook.spectrum.sample.model.IoType
import com.facebook.spectrum.sample.model.TranscodeState
import com.facebook.spectrum.sample.model.TranscodeViewModel
import java.io.ByteArrayOutputStream
import java.io.IOException

class TranscodeAsyncTask(
        private val context: Context,
        private val spectrum: Spectrum,
        private val transcodeViewModel: TranscodeViewModel,
        private val configurationViewModel: ConfigurationViewModel) : AsyncTask<Void, Void, SpectrumResult>() {

    private var outputStream: ByteArrayOutputStream? = null
    private var outputBitmap: Bitmap? = null

    override fun doInBackground(vararg params: Void?): SpectrumResult? {
        transcodeViewModel.transcodeState = TranscodeState.RUNNING

        return try {
            when (Pair(transcodeViewModel.inputType, transcodeViewModel.outputType)) {
                Pair(IoType.FILE, IoType.FILE) -> transcode()
                Pair(IoType.BITMAP, IoType.FILE) -> encode()
                Pair(IoType.FILE, IoType.BITMAP) -> decode()
                Pair(IoType.BITMAP, IoType.BITMAP) -> transform()
                else -> null
            }
        } catch (e: SpectrumException) {
            Log.e(transcodeViewModel.TAG, "Failed to transcode", e)
            transcodeViewModel.outputError = "${e.name}: ${e.message}"
            null
        } catch (e: IOException) {
            Log.e(transcodeViewModel.TAG, "Failed to access files", e)
            transcodeViewModel.outputError = "${e.message}"
            null
        }
    }

    private fun encode(): SpectrumResult {
        val optionsBuilder = EncodeOptions.Builder(makeEncodeRequirement(transcodeViewModel))
        applyModelToOptions(optionsBuilder)

        outputStream = ByteArrayOutputStream()
        return spectrum.encode(
                transcodeViewModel.inputImageBitmap,
                EncodedImageSink.from(outputStream),
                optionsBuilder.build(),
                "caller_context_encode")
    }

    private fun transcode(): SpectrumResult {
        val optionsBuilder = TranscodeOptions.Builder(makeEncodeRequirement(transcodeViewModel))
        applyModelToOptions(optionsBuilder)

        val inputStream = context.contentResolver.openInputStream(transcodeViewModel.inputImageUri!!)
        inputStream.use {
            outputStream = ByteArrayOutputStream()
            return spectrum.transcode(
                    EncodedImageSource.from(inputStream),
                    EncodedImageSink.from(outputStream),
                    optionsBuilder.build(),
                    "caller_context_transcode")
        }
    }

    private fun decode(): SpectrumResult {
        val optionsBuilder = DecodeOptions.Builder()
        applyModelToOptions(optionsBuilder)

        val inputStream = context.contentResolver.openInputStream(transcodeViewModel.inputImageUri!!)
        inputStream.use {
            val bitmapTarget = BitmapTarget()
            return spectrum.decode(
                    EncodedImageSource.from(inputStream),
                    bitmapTarget,
                    optionsBuilder.build(),
                    "caller_context_decode").also {
                outputBitmap = bitmapTarget.bitmap
            }
        }
    }

    private fun transform(): SpectrumResult {
        val optionsBuilder = TransformOptions.Builder()
        applyModelToOptions(optionsBuilder)

        val bitmapTarget = BitmapTarget()
        return spectrum.transform(
                transcodeViewModel.inputImageBitmap,
                bitmapTarget,
                optionsBuilder.build(),
                "caller_context_decode").also {
            outputBitmap = bitmapTarget.bitmap
        }
    }

    override fun onPostExecute(spectrumResult: SpectrumResult?) {
        Log.d(transcodeViewModel.TAG, "Spectrum result: $spectrumResult")
        if (spectrumResult == null) {
            transcodeViewModel.outputError = "spectrumResult == null"
            transcodeViewModel.transcodeState = TranscodeState.FINISHED_ERROR
        } else {
            transcodeViewModel.outputImageBitmap = outputBitmap
            transcodeViewModel.outputImageBytes = outputStream?.toByteArray()
            transcodeViewModel.transcodeState = TranscodeState.FINISHED_SUCCESS
        }
    }

    private fun <T : Options.Builder<T>?> applyModelToOptions(optionsBuilder: Options.Builder<T>) {
        transcodeViewModel.rotate.requirement?.let { optionsBuilder.rotate(it) }
        transcodeViewModel.resize.requirement?.let { optionsBuilder.resize(it) }
        transcodeViewModel.crop.requirement?.let { optionsBuilder.crop(it) }
        optionsBuilder.configuration(configurationViewModel.makeConfiguration())
    }

    private fun makeEncodeRequirement(transcodeViewModel: TranscodeViewModel): EncodeRequirement? {
        val mode = transcodeViewModel.encodeMode.mode
        val qualityLevel = when {
            TextUtils.isEmpty(transcodeViewModel.quality) -> null
            else -> Integer.parseInt(transcodeViewModel.quality)
        }

        return when (qualityLevel) {
            null -> EncodeRequirement(transcodeViewModel.outputFormat, mode)
            else -> EncodeRequirement(transcodeViewModel.outputFormat, qualityLevel, mode)
        }
    }
}
