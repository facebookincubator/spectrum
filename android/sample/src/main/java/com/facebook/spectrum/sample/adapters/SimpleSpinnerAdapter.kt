// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

package com.facebook.spectrum.sample.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.AdapterView
import android.widget.BaseAdapter
import android.widget.Spinner
import android.widget.TextView
import com.facebook.spectrum.sample.R
import com.facebook.spectrum.sample.model.Entry

open class SimpleSpinnerAdapter<T : Entry>(
        private val layoutInflater: LayoutInflater,
        private val entries: List<T>
) : BaseAdapter() {
    override fun getCount() = entries.size
    override fun getItem(position: Int) = entries[position]
    override fun getItemId(position: Int) = position.toLong()

    override fun getView(position: Int, convertView: View?, parent: ViewGroup): View {
        val layout = android.R.layout.simple_spinner_item
        return getViewInternal(position, convertView, parent, layout)
    }

    override fun getDropDownView(position: Int, convertView: View?, parent: ViewGroup): View {
        val layout = R.layout.spinner_dropdown_item
        return getViewInternal(position, convertView, parent, layout)
    }

    private fun getViewInternal(
            position: Int,
            convertView: View?,
            parent: ViewGroup,
            layout: Int
    ): View {
        val view = convertView ?: layoutInflater.inflate(layout, parent, false)

        val entry = entries[position]
        view.findViewById<TextView>(android.R.id.text1)?.let { it.text = entry.title }
        view.findViewById<TextView>(android.R.id.text2)?.let {
            it.text = entry.description
            it.visibility = if (entry.description != null) View.VISIBLE else View.GONE
        }

        return view
    }
}

fun <T : Entry> Spinner.configureSimpleSpinner(
        layoutInflater: LayoutInflater,
        entries: List<T>,
        onSelected: (T) -> Unit) {
    this.adapter = SimpleSpinnerAdapter(layoutInflater, entries)
    this.onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
        override fun onNothingSelected(parent: AdapterView<*>?) {}
        override fun onItemSelected(parent: AdapterView<*>?, view: View?, position: Int, id: Long) = onSelected(entries[position])
    }
}
