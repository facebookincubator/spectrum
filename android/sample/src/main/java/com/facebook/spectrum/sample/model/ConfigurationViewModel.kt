// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample.model

import com.facebook.spectrum.Configuration
import com.facebook.spectrum.image.ImageChromaSamplingMode
import com.facebook.spectrum.image.ImageColor

class ParameterGroup(val name: String, val parameters: Array<Parameter>)

abstract class Parameter(val description: String?) {
    abstract fun udpateConfigurationBuider(configurationBuilder: Configuration.Builder)
}

class BooleanParameter(
        description: String?,
        defaultValue: Boolean,
        private val setter: ((Configuration.Builder, Boolean?) -> Unit)) : Parameter(description) {
    var value: Boolean? = defaultValue
    fun set(v: Boolean) {
        value = v
    }

    override fun udpateConfigurationBuider(configurationBuilder: Configuration.Builder) = setter(configurationBuilder, value)
}

class EnumParameterEntry<T>(name: String, description: String?, val value: T?) : Entry(name, description)

class EnumParameter<T>(
        description: String?,
        val entries: Array<EnumParameterEntry<T>>,
        private val setter: ((Configuration.Builder, T?) -> Unit)) : Parameter(description) {
    var value: T? = entries[0].value
    fun set(v: EnumParameterEntry<out T?>) {
        value = v.value
    }

    override fun udpateConfigurationBuider(configurationBuilder: Configuration.Builder) = setter(configurationBuilder, value)
    fun getSelectedIndex(): Int = entries.indexOfFirst { it.value == value }
}

enum class ConfigurationViewModel {
    INSTANCE;

    fun set(parameter: BooleanParameter, checked: Boolean) {
        parameter.set(checked)
    }

    fun <T> set(parameter: EnumParameter<T>, entry: EnumParameterEntry<out T?>) {
        parameter.set(entry)
    }

    fun makeConfiguration(): Configuration {
        val configurationBuilder = Configuration.Builder()
        groups.forEach { group -> group.parameters.forEach { parameter -> parameter.udpateConfigurationBuider(configurationBuilder) } }
        return configurationBuilder.build()
    }

    private val colorEntries = arrayOf(
            EnumParameterEntry("Black", "#000000", ImageColor(0, 0, 0)),
            EnumParameterEntry("White", "#FFFFFF", ImageColor(255, 255, 255)),
            EnumParameterEntry("Red", "#FF0000", ImageColor(255, 0, 0)),
            EnumParameterEntry("Green", "#00FF00", ImageColor(0, 255, 0)),
            EnumParameterEntry("Blue", "#0000FF", ImageColor(0, 0, 255))
    )

    private val chromaSamplingOverrideEntries: Array<EnumParameterEntry<ImageChromaSamplingMode>> = arrayOf(
            EnumParameterEntry("No override", null, null as ImageChromaSamplingMode?),
            EnumParameterEntry("4:2:0", "4 luma information per chroma information", ImageChromaSamplingMode.S420),
            EnumParameterEntry("4:2:2", "2 luma information per chroma information", ImageChromaSamplingMode.S422),
            EnumParameterEntry("4:4:4", "1 luma information per chroma information", ImageChromaSamplingMode.S444)
    )

    private val pngCompressionLevel: Array<EnumParameterEntry<Int>> = arrayOf(
            EnumParameterEntry("Default", null, null as Int?),
            EnumParameterEntry("Compression level = 0", "No compression", 0),
            EnumParameterEntry("Compression level = 1", "Best speed", 1),
            EnumParameterEntry("Compression level = 9", "Best compression", 9)
    )

    private val webpMethodEntries: Array<EnumParameterEntry<Int>> = arrayOf(
            EnumParameterEntry("Default", null, null as Int?),
            EnumParameterEntry("Webp method = 0", "Slowest", 0),
            EnumParameterEntry("Webp method = 3", "Balanced", 3),
            EnumParameterEntry("Webp method = 6", "Fastest", 6)
    )

    private val webpImageHint: Array<EnumParameterEntry<Configuration.ImageHint>> = arrayOf(
            EnumParameterEntry("Default", null, Configuration.ImageHint.DEFAULT),
            EnumParameterEntry("Picture", "General indoor digital picture", Configuration.ImageHint.PICTURE),
            EnumParameterEntry("Photo", "General outdoor digital picture", Configuration.ImageHint.PHOTO),
            EnumParameterEntry("Graph", "Graphical image ", Configuration.ImageHint.GRAPH)
    )

    val groups = arrayOf(
            ParameterGroup("General", arrayOf(
                    EnumParameter("Background color", colorEntries) { cb, v -> cb.setDefaultBackgroundColor(v) },
                    BooleanParameter("Interpret metadata", true) { cb, v -> cb.setInterpretMetadata(v) },
                    EnumParameter("Chroma Sampling override", chromaSamplingOverrideEntries) { cb, v -> cb.setChromaSamplingModeOverride(v) },
                    BooleanParameter("Propagate chroma sampling", false) { cb, v -> cb.setPropagateChromaSamplingModeFromSource(v) })
            ),
            ParameterGroup("Jpeg", arrayOf(
                    BooleanParameter("Use Trellis optimization", true) { cb, v -> cb.setUseTrellis(v) },
                    BooleanParameter("Progressive encoding", true) { cb, v -> cb.setUseProgressive(v) },
                    BooleanParameter("Use scan optimization", true) { cb, v -> cb.setUseOptimizeScan(v) },
                    BooleanParameter("Compatible DC scan", true) { cb, v -> cb.setUseCompatibleDcScanOpt(v) },
                    BooleanParameter("PSNR optimised quantisation table", false) { cb, v -> cb.setUsePsnrQuantTable(v) })
            ),
            ParameterGroup("Png", arrayOf(
                    BooleanParameter("Save with interlacing", false) { cb, v -> cb.setUseInterlacing(v) },
                    EnumParameter("Compression level", pngCompressionLevel) { cb, v -> v?.let { cb.setCompressionLevel(v) } })
            ),
            ParameterGroup("WebP", arrayOf(
                    EnumParameter("Webp compression method", webpMethodEntries) { cb, v -> v?.let { cb.setWebpMethod(it) } },
                    EnumParameter("Webp image hint", webpImageHint) { cb, v -> cb.setWebpImageHint(v) }

            ))
    )

}
