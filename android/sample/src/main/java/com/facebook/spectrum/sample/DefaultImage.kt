// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample

import android.content.Context
import android.net.Uri
import java.io.File
import java.io.FileOutputStream

private fun localFilePath(context: Context) = File(context.filesDir, "default_image.jpg")

private fun ensureFileOnDisk(context: Context, localPath: File) {
    if (!localPath.exists()) {
        context.resources.openRawResource(R.raw.default_image).copyTo(FileOutputStream(localPath))
    }
}

@JvmName("getDefaultImageUri")
fun defaultImageUri(context: Context) : Uri {
    val localPath = localFilePath(context)
    ensureFileOnDisk(context, localPath)
    return Uri.fromFile(localPath)
}
