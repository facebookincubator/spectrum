// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

final class SprectrumView: UIView, PickerButtonDelegate, UIGestureRecognizerDelegate {
  // MARK: - Public Properties

  var viewModel: SpectrumViewModel? {
    didSet {
      guard self.viewModel !== oldValue else { return }
      self.viewModel.flatMap(self.updateView(with:))
    }
  }

  // MARK: - Private Properties

  // MARK: IBOutlet

  @IBOutlet private var scrollView: UIScrollView!
  @IBOutlet private var stackView: UIStackView!
  @IBOutlet private var sourceImageView: UIImageView!
  @IBOutlet private var sourceImageButton: UIButton!
  @IBOutlet private var inputTypeButton: UIButton!
  @IBOutlet private var outputTypeButton: UIButton!
  @IBOutlet private var outputFormatButton: UIButton!
  @IBOutlet private var outputFormatContainerView: UIView!
  @IBOutlet private var compressionModeButton: UIButton!
  @IBOutlet private var modeContainerView: UIView!
  @IBOutlet private var qualityContainerView: UIView!
  @IBOutlet private var qualityButton: PickerButton!
  @IBOutlet private var rotationButton: UIButton!
  @IBOutlet private var scalingButton: UIButton!
  @IBOutlet private var croppingButton: UIButton!
  @IBOutlet private var transcodeButton: UIButton!

  // MARK: - NSObject

  deinit {
    NotificationCenter.default.removeObserver(self)
  }

  override func awakeFromNib() {
    super.awakeFromNib()

    self.qualityButton.delegate = self
    self.qualityButton.pickerOptions = SpectrumViewModel.qualityPickerOptions
    self.transcodeButton.layer.cornerRadius = 5

    NotificationCenter.default.addObserver(self, selector: #selector(self.didReceiveKeyboardNotification(_:)), name: UIResponder.keyboardWillShowNotification, object: nil)
    NotificationCenter.default.addObserver(self, selector: #selector(self.didReceiveKeyboardNotification(_:)), name: UIResponder.keyboardWillHideNotification, object: nil)
  }

  // MARK: - PickerButtonDelegate

  func pickerButton(_: UIButton, didPick option: Int) {
    self.viewModel!.quality = option
  }

  // MARK: - UIGestureRecognizerDelegate

  func gestureRecognizer(_: UIGestureRecognizer, shouldRecognizeSimultaneouslyWith _: UIGestureRecognizer) -> Bool {
    return true
  }

  // MARK: - Public Methods

  func updateView(with viewModel: SpectrumViewModel) {
    self.sourceImageView.image = viewModel.source.image
    self.sourceImageButton.setTitle(viewModel.sourceImageDescription, for: .normal)
    self.inputTypeButton.setTitle(viewModel.inputType.title, for: .normal)
    self.outputTypeButton.setTitle(viewModel.outputType.title, for: .normal)
    self.outputFormatButton.setTitle(viewModel.outputFormat.title, for: .normal)
    self.compressionModeButton.setTitle(viewModel.compressionMode.title, for: .normal)
    self.qualityButton.currentOption = viewModel.quality
    self.rotationButton.setTitle(viewModel.rotation.title, for: .normal)
    self.scalingButton.setTitle(viewModel.scaling.title, for: .normal)
    self.croppingButton.setTitle(viewModel.cropping.title, for: .normal)
    self.transcodeButton.isEnabled = viewModel.isTranscodeButtonEnabled
    UIView.performWithoutAnimation {
      self.transcodeButton.setTitle(viewModel.actionButtonTitle, for: .normal)
      self.transcodeButton.layoutIfNeeded()
    }

    self.stackView.isUserInteractionEnabled = viewModel.isTranscoding == false

    if self.modeContainerView.isHidden == viewModel.showsEncodingMode {
      UIView.animate(withDuration: 0.4) {
        self.modeContainerView.isHidden = viewModel.showsEncodingMode == false
      }
    }

    if self.qualityContainerView.isHidden == viewModel.showsQuality {
      UIView.animate(withDuration: 0.4) {
        self.qualityContainerView.isHidden = viewModel.showsQuality == false
      }
    }

    if self.outputFormatContainerView.isHidden == viewModel.showsOutputFormat {
      UIView.animate(withDuration: 0.4) {
        self.outputFormatContainerView.isHidden = viewModel.showsOutputFormat == false
      }
    }
  }

  // MARK: - Private Methods

  @objc private func didReceiveKeyboardNotification(_ notification: Notification) {
    guard let keyboardFrame = (notification.userInfo?[UIResponder.keyboardFrameEndUserInfoKey] as? NSValue)?.cgRectValue else {
      return
    }

    let intersection = self.convert(self.scrollView.frame, to: nil).intersection(keyboardFrame)
    self.scrollView.contentInset.bottom = intersection.height
  }

  // MARK: IBActions

  @IBAction private func didTapQualityButton(_ sender: UIButton) {
    sender.toggleFirstResponder()
  }

  @IBAction private func didTapBackgroundView(_ gestureRecognizer: UITapGestureRecognizer) {
    // We should only end editing if the tap isn't in the quality button's bounds
    // as its handler already toggles its first responder state. Guarding this handler
    // prevents from making any assumptions in regards to the order in which the gesture
    // and buttons handlers will be called.
    let locationInQualityButton = gestureRecognizer.location(in: self.qualityButton)
    guard self.qualityButton.bounds.contains(locationInQualityButton) == false else { return }
    self.endEditing(true)
  }

  @IBAction private func didTapTranscodeButton(_: UIButton) {
    self.viewModel!.transcode()
  }
}

private extension UIResponder {
  func toggleFirstResponder() {
    if self.isFirstResponder {
      self.resignFirstResponder()
    } else {
      self.becomeFirstResponder()
    }
  }
}
