// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.os.Looper
import android.provider.MediaStore.Images.Media.*
import android.transition.TransitionManager
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.facebook.spectrum.DefaultPlugins
import com.facebook.spectrum.Spectrum
import com.facebook.spectrum.logging.SpectrumLogcatLogger
import com.facebook.spectrum.requirements.EncodeRequirement
import com.facebook.spectrum.sample.adapters.configureSimpleSpinner
import com.facebook.spectrum.sample.model.*
import kotlinx.android.synthetic.main.main_activity.*
import java.io.IOException

private fun makeSpectrum(): Spectrum {
    return Spectrum.make(SpectrumLogcatLogger(Log.INFO), DefaultPlugins.get())
}

private fun transcode(
        context: Context,
        spectrum: Spectrum,
        transcoderViewModel: TranscodeViewModel,
        configurationViewModel: ConfigurationViewModel) =
        TranscodeAsyncTask(context, spectrum, transcoderViewModel, configurationViewModel).execute()

class MainActivity : AppCompatActivity() {
    private val spectrum: Spectrum = makeSpectrum()
    private val transcodeModel = TranscodeViewModel.INSTANCE
    private val configurationModel = ConfigurationViewModel.INSTANCE
    private val requestCodeMediaPick: Int = 1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.main_activity)

        button_choose_input.setOnClickListener {
            val intent = Intent(Intent.ACTION_PICK, EXTERNAL_CONTENT_URI)
            startActivityForResult(intent, requestCodeMediaPick)
        }

        transcodeModel.listener = { x -> reloadFromModel(x) }
        setInputImage(defaultImageUri(this))

        button_run_transcode.setOnClickListener { transcode(this@MainActivity, spectrum, transcodeModel, configurationModel) }

        spinner_input_type.configureSimpleSpinner(layoutInflater, IoTypeSpinnerEntry.allEntries) { transcodeModel.inputType = it.ioType }
        spinner_output_type.configureSimpleSpinner(layoutInflater, IoTypeSpinnerEntry.allEntries) { transcodeModel.outputType = it.ioType }
        spinner_output_format.configureSimpleSpinner(layoutInflater, ImageTypeSpinnerEntry.allEntries) { transcodeModel.outputFormat = it.encodedImageFormat }
        spinner_encode_mode.configureSimpleSpinner(layoutInflater, EncodeModeSpinnerEntry.allEntries) { transcodeModel.encodeMode = it.encodeMode }
        spinner_orientation.configureSimpleSpinner(layoutInflater, OrientationSpinnerEntry.allEntries) { transcodeModel.rotate = it.rotate }
        spinner_scaling.configureSimpleSpinner(layoutInflater, ResizeEntry.allEntries) { transcodeModel.resize = it.resize }
        spinner_cropping.configureSimpleSpinner(layoutInflater, CropSpinnerEntry.allEntries) { transcodeModel.crop = it.crop }

        edittext_quality_level.setOnEditorActionListener { v, _, _ ->
            transcodeModel.quality = v.text.toString()
            true
        }

        button_edit_configuration.setOnClickListener { startActivity(Intent(this, ConfigurationActivity::class.java)) }

        reloadFromModel(transcodeModel)
    }

    private fun openComparison() {
        ComparisonViewModel.INSTANCE.updateFromTranscodeModel(transcodeModel)
        transcodeModel.reset()
        startActivity(Intent(this, ComparisonActivity::class.java))
    }

    private fun setInputImage(uri: Uri) {
        try {
            transcodeModel.setInputImage(this, uri)
        } catch (e: IOException) {
            Toast.makeText(this, "failed to load: $uri", Toast.LENGTH_SHORT).show()
        }
    }

    private fun startAnimation() = TransitionManager.beginDelayedTransition(options_root)
    private fun showError(errorMessage: String) = runOnUiThread { Toast.makeText(applicationContext, errorMessage, Toast.LENGTH_SHORT).show() }
    private fun isUiThread() = Looper.getMainLooper().thread !== Thread.currentThread()

    private fun reloadFromModel(model: TranscodeViewModel) {
        if (isUiThread()) {
            return runOnUiThread { reloadFromModel(model) }
        }

        spinner_input_type.setSelection(if (model.inputType === IoType.FILE) 0 else 1)
        spinner_output_type.setSelection(if (model.outputType === IoType.FILE) 0 else 1)
        output_format_container.visibility = when {
            model.outputType === IoType.FILE -> View.VISIBLE
            else -> View.GONE
        }
        edittext_quality_level.setText(model.quality)
        source_info.text = model.sourceSummary
        source_image.setImageBitmap(model.inputImageBitmap)

        startAnimation()

        val outputOptionsVisibility = when (model.outputType) {
            IoType.FILE -> View.VISIBLE
            else -> View.GONE
        }
        encode_mode_container.visibility = outputOptionsVisibility

        // Some options disabled when outputting to Bitmap or when lossless is enabled
        val outputQualityVisibility = when {
            outputOptionsVisibility == View.VISIBLE && model.encodeMode.mode != EncodeRequirement.Mode.LOSSLESS -> View.VISIBLE
            else -> View.GONE
        }
        quality_level_container.visibility = outputQualityVisibility

        val state = model.transcodeState
        button_run_transcode.isEnabled = state !== TranscodeState.RUNNING
        button_run_transcode.setText(
                if (state == TranscodeState.RUNNING) R.string.button_transcoding
                else R.string.button_run_transcode)

        when (state) {
            TranscodeState.FINISHED_ERROR -> {
                showError("Failed to transcode: ${model.outputError}")
                model.reset()
            }
            TranscodeState.FINISHED_SUCCESS -> openComparison()
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        when (requestCode) {
            requestCodeMediaPick -> when (resultCode) {
                Activity.RESULT_OK -> setInputImage(data?.data!!)
                else -> Toast.makeText(this, "Media pick failed", Toast.LENGTH_SHORT).show()
            }
            else -> super.onActivityResult(requestCode, resultCode, data)
        }
    }
}
