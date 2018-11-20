// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample.model

import com.facebook.spectrum.image.EncodedImageFormat
import com.facebook.spectrum.image.ImageSize
import com.facebook.spectrum.requirements.CropRequirement
import com.facebook.spectrum.requirements.EncodeRequirement
import com.facebook.spectrum.requirements.ResizeRequirement
import com.facebook.spectrum.requirements.ResizeRequirement.*
import com.facebook.spectrum.requirements.RotateRequirement

open class Entry(val title: String, val description: String? = null)

//
// IoType
//

enum class IoType constructor(val title: String, val description: String) {
    FILE("File", "Encoded data"),
    BITMAP("Bitmap", "Pixel data"),
}

data class IoTypeSpinnerEntry(val ioType: IoType) : Entry(ioType.title, ioType.description) {
    companion object {
        val allEntries = IoType.values().map { IoTypeSpinnerEntry(it) }
    }
}

//
// Crop
//

enum class Crop constructor(val title: String, val description: String?, val requirement: CropRequirement?) {
    NO_CROP(
            "No crop",
            null,
            null),
    CROP_1(
            "Absolute crop 1",
            "absolute (50, 50, 100, 100)",
            CropRequirement.makeAbsoluteToOrigin(50, 50, 100, 100, true)),
    CROP_2(
            "Absolute crop 2",
            "absolute (0, 0, 300, 50)",
            CropRequirement.makeAbsoluteToOrigin(0, 0, 300, 50, true)),
    CROP_3(
            "Relative crop top-left",
            "relative (0f, 0f, 0.5f, 0.5f)",
            CropRequirement.makeRelativeToOrigin(0f, 0f, 0.5f, 0.5f, true)),
    CROP_4(
            "Relative crop top-left approx.",
            "relative ~(0f, 0f, 0.5f, 0.5f)",
            CropRequirement.makeRelativeToOrigin(0f, 0f, 0.5f, 0.5f, false)),
    CROP_5(
            "Relative crop center approx.",
            "relative ~(0.25f, 0.25f, 0.75f, 0.75f)",
            CropRequirement.makeRelativeToOrigin(0.25f, 0.25f, 0.75f, 0.75f, false)),
}

data class CropSpinnerEntry(val crop: Crop) : Entry(crop.title, crop.description) {
    companion object {
        val allEntries = Crop.values().map { CropSpinnerEntry(it) }
    }
}

//
// Image type
//

data class ImageTypeSpinnerEntry(val encodedImageFormat: EncodedImageFormat) : Entry(encodedImageFormat.toString().toUpperCase()) {
    companion object {
        val allEntries = arrayOf(
                EncodedImageFormat.JPEG,
                EncodedImageFormat.PNG,
                EncodedImageFormat.WEBP
        ).map { ImageTypeSpinnerEntry(it) }
    }
}

//
// Rotate
//

enum class Rotate constructor(val title: String, val requirement: RotateRequirement?) {
    NO_ROTATION("No rotation", null),
    ROTATE_90("Rotate 90", RotateRequirement(90)),
    ROTATE_180("Rotate 180", RotateRequirement(180)),
    ROTATE_270("Rotate 270", RotateRequirement(270)),
    FLIP_HORIZONTALLY("Flip horizontally", RotateRequirement(0, true, false, false)),
    FLIP_VERTICALLY("Flip vertically", RotateRequirement(0, false, true, false)),
}

data class OrientationSpinnerEntry(val rotate: Rotate) : Entry(rotate.title) {
    companion object {
        val allEntries = Rotate.values().map { OrientationSpinnerEntry(it) }
    }
}

//
// Scaling
//

enum class Resize constructor(val title: String, val requirement: ResizeRequirement?) {
    NO_RESIZE(
            "No resize",
            null),
    EXACT_2X2(
            "Exact 2x2",
            ResizeRequirement(Mode.EXACT, ImageSize(2, 2))),
    EXACT_200X200(
            "Exact 200x200",
            ResizeRequirement(Mode.EXACT, ImageSize(200, 200))),
    EXACT_1000X1000(
            "Exact 1000x1000",
            ResizeRequirement(Mode.EXACT, ImageSize(1000, 1000))),
    EXACT_OR_SMALLER_200X200(
            "ExactOrSmaller 200x200",
            ResizeRequirement(Mode.EXACT_OR_SMALLER, ImageSize(200, 200))),
    EXACT_OR_SMALLER_1000X1000(
            "ExactOrSmaller 1000x1000",
            ResizeRequirement(Mode.EXACT_OR_SMALLER, ImageSize(1000, 1000))),
}

data class ResizeEntry(val resize: Resize) : Entry(resize.title) {
    companion object {
        val allEntries = Resize.values().map { ResizeEntry(it) }
    }
}

//
// Encode Mode
//

enum class EncodeMode constructor(val title: String, val description: String, val mode: EncodeRequirement.Mode) {
    ANY("Any", "Can be lossy or lossless", EncodeRequirement.Mode.ANY),
    LOSSY("Lossy", "Must be lossy", EncodeRequirement.Mode.LOSSY),
    LOSSLESS("Lossless", "Must be lossless", EncodeRequirement.Mode.LOSSLESS),
}

data class EncodeModeSpinnerEntry(val encodeMode: EncodeMode) : Entry(encodeMode.title, encodeMode.description) {
    companion object {
        val allEntries = EncodeMode.values().map { EncodeModeSpinnerEntry(it) }
    }
}
