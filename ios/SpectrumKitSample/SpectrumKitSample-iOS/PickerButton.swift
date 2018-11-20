// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

protocol PickerButtonDelegate: class {
  func pickerButton(_ pickerButton: UIButton, didPick option: Int)
}

final class PickerButton: UIButton, UIPickerViewDataSource, UIPickerViewDelegate {
  // MARK: - Private Properties

  private let pickerView = UIPickerView()
  private let toolbar = UIToolbar()

  // MARK: - Public Properties

  weak var delegate: PickerButtonDelegate?

  var pickerOptions: [Int] = [] {
    didSet {
      self.pickerView.reloadAllComponents()
    }
  }

  var currentOption: Int? {
    didSet {
      if let currentOption = self.currentOption {
        guard let optionIndex = self.currentOption.flatMap(self.pickerOptions.index(of:)) else {
          fatalError("Unable to find current option in pickerOptions")
        }

        self.pickerView.selectRow(optionIndex, inComponent: 0, animated: false)
        self.setTitle(String(currentOption), for: .normal)
      } else {
        self.setTitle(NSLocalizedString("Unset", comment: "Picker button no option"), for: .normal)
      }
    }
  }

  // MARK: - NSObject

  required init?(coder aDecoder: NSCoder) {
    super.init(coder: aDecoder)

    self.pickerView.tintColor = .tintColor
    self.pickerView.delegate = self
    self.pickerView.dataSource = self
    self.pickerView.autoresizingMask = [.flexibleWidth, .flexibleHeight]
    self.toolbar.tintColor = .tintColor
    self.toolbar.autoresizingMask = [.flexibleHeight, .flexibleWidth]
    self.toolbar.items = [
      UIBarButtonItem(barButtonSystemItem: .flexibleSpace, target: nil, action: nil),
      UIBarButtonItem(barButtonSystemItem: .done, target: self, action: #selector(self.didTapDoneBarButtonItem(_:))),
    ]
  }

  // MARK: - UIView

  override init(frame _: CGRect) {
    fatalError("init(frame:) not implemented")
  }

  // MARK: - UIResponder

  override var canBecomeFirstResponder: Bool {
    return true
  }

  override var inputView: UIView? {
    return self.pickerView
  }

  override var inputAccessoryView: UIView? {
    return self.toolbar
  }

  // MARK: - UIPickerViewDelegate

  func pickerView(_: UIPickerView, titleForRow row: Int, forComponent _: Int) -> String? {
    return self.pickerOptions[row].description
  }

  func pickerView(_: UIPickerView, didSelectRow row: Int, inComponent _: Int) {
    self.delegate?.pickerButton(self, didPick: self.pickerOptions[row])
  }

  // MARK: - UIPickerViewDataSource

  func numberOfComponents(in _: UIPickerView) -> Int {
    return 1
  }

  func pickerView(_: UIPickerView, numberOfRowsInComponent _: Int) -> Int {
    return self.pickerOptions.count
  }

  // MARK: - Private Methods

  @objc private func didTapDoneBarButtonItem(_: UIBarButtonItem) {
    self.resignFirstResponder()
  }
}
