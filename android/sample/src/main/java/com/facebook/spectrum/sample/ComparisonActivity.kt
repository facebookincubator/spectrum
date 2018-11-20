// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample

import android.os.Bundle
import android.util.DisplayMetrics
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import android.view.View
import android.widget.SeekBar
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.doOnNextLayout
import com.facebook.spectrum.sample.model.ComparisonViewModel
import kotlinx.android.synthetic.main.comparison_activity.*
import kotlin.math.min

class ComparisonActivity : AppCompatActivity() {
    private val mDisplayMetrics = DisplayMetrics()

    private var mScaleFactor = 1.0f
    private var mScaleGestureDetector: ScaleGestureDetector? = null
    private var mGestureDetector: GestureDetector? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.comparison_activity)
        supportActionBar?.hide()

        mScaleGestureDetector = ScaleGestureDetector(this, ScaleListener())
        mGestureDetector = GestureDetector(this, GestureListener())

        new_image.doOnNextLayout {
            val parent = new_image.parent as View
            mDisplayMetrics.widthPixels = parent.width
            mDisplayMetrics.heightPixels = parent.height
        }

        mDisplayMetrics.widthPixels = (new_image.parent as View).width
        mDisplayMetrics.heightPixels = (new_image.parent as View).height

        val model = ComparisonViewModel.INSTANCE
        new_image.setImageBitmap(model.outputBitmap)
        old_image.setImageBitmap(model.inputBitmap)

        comparison_bar.setOnSeekBarChangeListener(
                object : SeekBar.OnSeekBarChangeListener {
                    override fun onProgressChanged(seekBar: SeekBar, progressValue: Int, fromUser: Boolean) {
                        new_image.alpha = progressValue.toFloat() / 100f
                    }

                    override fun onStartTrackingTouch(seekBar: SeekBar) {}
                    override fun onStopTrackingTouch(seekBar: SeekBar) {}
                })
    }

    override fun onTouchEvent(ev: MotionEvent): Boolean {
        mScaleGestureDetector?.onTouchEvent(ev)
        mGestureDetector?.onTouchEvent(ev)
        return true
    }

    private inner class ScaleListener : ScaleGestureDetector.SimpleOnScaleGestureListener() {
        override fun onScale(detector: ScaleGestureDetector): Boolean {
            mScaleFactor = (mScaleFactor * detector.scaleFactor).coerceIn(1f, 10f)

            new_image.scaleX = mScaleFactor
            new_image.scaleY = mScaleFactor
            old_image.scaleX = mScaleFactor
            old_image.scaleY = mScaleFactor

            setImageTranslation(0f, 0f)
            return true
        }
    }

    private inner class GestureListener : GestureDetector.SimpleOnGestureListener() {
        override fun onScroll(e1: MotionEvent, e2: MotionEvent, distanceX: Float, distanceY: Float): Boolean {
            setImageTranslation(distanceX, distanceY)
            return true
        }
    }

    private fun setImageTranslation(distanceX: Float, distanceY: Float) {
        val scaledImageWidth = new_image.width * mScaleFactor
        val scaledImageHeight = new_image.height * mScaleFactor
        val limitWidth = mDisplayMetrics.widthPixels.toFloat()
        val limitHeight = mDisplayMetrics.heightPixels.toFloat()
        var newX = new_image.translationX - distanceX
        var newY = new_image.translationY - distanceY

        val leftEdge = newX - (scaledImageWidth - limitWidth) / 2
        val topEdge = newY - (scaledImageHeight - limitHeight) / 2

        newX += getOffset(leftEdge, leftEdge + scaledImageWidth, 0f, limitWidth, leftEdge + scaledImageWidth / 2)
        newY += getOffset(topEdge, topEdge + scaledImageHeight, 0f, limitHeight, topEdge + scaledImageHeight / 2)

        new_image.translationX = newX;
        new_image.translationY = newY
        old_image.translationX = newX
        old_image.translationY = newY
    }

    private fun mean(a: Float, b: Float) = (a + b) / 2

    private fun getOffset(
            imageStart: Float, imageEnd: Float, limitStart: Float, limitEnd: Float, limitCenter: Float): Float {
        val imageWidth = imageEnd - imageStart
        val limitWidth = limitEnd - limitStart
        val limitInnerWidth = min(limitCenter - limitStart, limitEnd - limitCenter) * 2
        // center if smaller than limitInnerWidth
        if (imageWidth < limitInnerWidth) {
            return limitCenter - mean(imageEnd, imageStart)
        }
        // to the edge if in between and limitCenter is not (limitLeft + limitRight) / 2
        if (imageWidth < limitWidth) {
            return when {
                limitCenter < mean(limitStart, limitEnd) -> limitStart - imageStart
                else -> limitEnd - imageEnd
            }
        }
        // to the edge if larger than limitWidth and empty space visible
        return when {
            imageStart > limitStart -> limitStart - imageStart
            imageEnd < limitEnd -> limitEnd - imageEnd
            else -> 0f
        }
    }
}
