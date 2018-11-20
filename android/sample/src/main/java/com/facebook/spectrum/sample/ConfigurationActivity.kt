// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample

import android.os.Bundle
import android.view.View
import android.view.ViewGroup
import android.widget.Spinner
import android.widget.Switch
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.facebook.spectrum.sample.adapters.configureSimpleSpinner
import com.facebook.spectrum.sample.model.BooleanParameter
import com.facebook.spectrum.sample.model.ConfigurationViewModel
import com.facebook.spectrum.sample.model.EnumParameter

import kotlinx.android.synthetic.main.configuration_activity.*

class ConfigurationActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.configuration_activity)
    }

    override fun onStart() {
        super.onStart()
        loadFromModel(ConfigurationViewModel.INSTANCE)
    }

    private fun loadFromModel(model: ConfigurationViewModel) {
        val rootView = configuration_root
        model.groups.forEach { group ->
            val groupLayout = layoutInflater.inflate(R.layout.configuration_group, rootView, false) as ViewGroup
            groupLayout.findViewById<TextView>(R.id.configuration_group_header).text = group.name.toUpperCase()

            group.parameters.forEach { parameter ->
                val parameterLayout = when (parameter) {
                    is BooleanParameter -> createBooleanParameterView(groupLayout, parameter, model)
                    is EnumParameter<*> -> createEnumParameterView(groupLayout, parameter, model)
                    else -> TODO("unexpected parameter type")
                }
                groupLayout.addView(parameterLayout)
            }
            rootView.addView(groupLayout)
        }
    }

    private fun <T> createEnumParameterView(groupLayout: ViewGroup, parameter: EnumParameter<T>, model: ConfigurationViewModel): View? {
        return layoutInflater.inflate(R.layout.configuration_group_parameter_enum, groupLayout, false).also {
            it.findViewById<TextView>(R.id.configuration_parameter_enum_text).text = parameter.description
            it.findViewById<Spinner>(R.id.configuration_parameter_enum_spinner).also {spinner ->
                spinner.configureSimpleSpinner(layoutInflater, parameter.entries.toList()) {model.set(parameter, it)}
                spinner.setSelection(parameter.getSelectedIndex())
            }
        }
    }

    private fun createBooleanParameterView(groupLayout: ViewGroup, parameter: BooleanParameter, model: ConfigurationViewModel): View? {
        return layoutInflater.inflate(R.layout.configuration_group_parameter_boolean, groupLayout, false).also {
            it.findViewById<TextView>(R.id.configuration_parameter_boolean_text).text = parameter.description
            it.findViewById<Switch>(R.id.configuration_parameter_boolean_switch).also {
                it.isChecked = parameter.value ?: false
                it.setOnCheckedChangeListener { _, isChecked -> model.set(parameter, isChecked) }
            }
        }
    }
}
