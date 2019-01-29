// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

final class ConfigurationViewController: UIViewController, ConfigurationViewModelDelegate {
  // MARK: - Public Properties

  var viewModel: ConfigurationViewModel? {
    didSet {
      guard self.viewModel !== oldValue else { return }

      if oldValue?.delegate === self {
        oldValue?.delegate = nil
      }

      self.viewModel?.delegate = self

      self.setupBindings()
      self.updateView()
    }
  }

  // MARK: - Private Properties

  private var alertOptionControllerByButton = [UIButton: AnyAlertOptionController]()
  private var keyPathsBySwitch: [UISwitch: ValueHandler<Bool>] = [:]

  @IBOutlet private var generalDefaultBackgroundColorButton: UIButton!
  @IBOutlet private var generalInterpretMetadataSwitch: UISwitch!
  @IBOutlet private var propagateChromaSamplingModeFromSourceSwitch: UISwitch!
  @IBOutlet private var jpegUseTrellisSwitch: UISwitch!
  @IBOutlet private var jpegUseProgressiveSwitch: UISwitch!
  @IBOutlet private var jpegUseOptimizeScanSwitch: UISwitch!
  @IBOutlet private var jpegUseDcScanOptionSwitch: UISwitch!
  @IBOutlet private var jpegUsePsnrQuantTableSwitch: UISwitch!
  @IBOutlet private var pngUseInterlacingSwitch: UISwitch!
  @IBOutlet private var pngCompressionLevelButton: UIButton!
  @IBOutlet private var webpMethodButton: UIButton!
  @IBOutlet private var webpImageHintButton: UIButton!

  // MARK: - UIViewController

  override func viewDidLoad() {
    super.viewDidLoad()

    guard self.viewModel != nil else {
      fatalError("View model should be set by viewDidLoad()")
    }

    self.setupBindings()
    self.updateView()
  }

  // MARK: - ConfigurationViewModelDelegate

  func viewModelDidUpdate(_: ConfigurationViewModel) {
    self.updateView()
  }

  // MARK: IBActions

  @IBAction private func didTapImageOptionsButton(_ sender: UIButton) {
    self.alertOptionControllerByButton[sender]!.presentAlert(from: self, sourceView: sender)
  }

  @IBAction private func didToggleSwitch(_ sender: UISwitch) {
    self.keyPathsBySwitch[sender]!.update(sender.isOn)
  }

  // MARK: - Private Methods

  private func setupBindings() {
    guard let viewModel = self.viewModel, self.isViewLoaded else {
      self.alertOptionControllerByButton = [:]
      self.keyPathsBySwitch = [:]
      return
    }

    self.alertOptionControllerByButton = [
      self.generalDefaultBackgroundColorButton: AnyAlertOptionController(alertContent: Alerts.configurationGeneralDefaultBackgroundColor, destination: viewModel.general, keyPath: \.defaultBackgroundColor),
      self.pngCompressionLevelButton: AnyAlertOptionController(alertContent: Alerts.configurationPngCompressionLevel, destination: viewModel.png, keyPath: \.compressionLevel),
      self.webpMethodButton: AnyAlertOptionController(alertContent: Alerts.configurationWebpMethod, destination: viewModel.webp, keyPath: \.method),
      self.webpImageHintButton: AnyAlertOptionController(alertContent: Alerts.configurationWebpImageHint, destination: viewModel.webp, keyPath: \.imageHint),
    ]

    self.keyPathsBySwitch = [
      self.generalInterpretMetadataSwitch: ValueHandler(destination: viewModel.general, keyPath: \.interpretMetadata),
      self.propagateChromaSamplingModeFromSourceSwitch: ValueHandler(destination: viewModel.general, keyPath: \.propagateChromaSamplingModeFromSource),
      self.jpegUseTrellisSwitch: ValueHandler(destination: viewModel.jpeg, keyPath: \.useTrellis),
      self.jpegUseProgressiveSwitch: ValueHandler(destination: viewModel.jpeg, keyPath: \.useProgressive),
      self.jpegUseOptimizeScanSwitch: ValueHandler(destination: viewModel.jpeg, keyPath: \.useOptimizeScan),
      self.jpegUseDcScanOptionSwitch: ValueHandler(destination: viewModel.jpeg, keyPath: \.useCompatibleDCScanOption),
      self.jpegUsePsnrQuantTableSwitch: ValueHandler(destination: viewModel.jpeg, keyPath: \.usePSNRQuantTable),
      self.pngUseInterlacingSwitch: ValueHandler(destination: viewModel.png, keyPath: \.useInterlacing),
    ]
  }

  private func updateView() {
    guard let viewModel = self.viewModel, self.isViewLoaded else { return }

    self.generalDefaultBackgroundColorButton.setTitle(viewModel.general.defaultBackgroundColor.title, for: .normal)
    self.generalInterpretMetadataSwitch.isOn = viewModel.general.interpretMetadata
    self.propagateChromaSamplingModeFromSourceSwitch.isOn = viewModel.general.propagateChromaSamplingModeFromSource

    self.jpegUseTrellisSwitch.isOn = viewModel.jpeg.useTrellis
    self.jpegUseProgressiveSwitch.isOn = viewModel.jpeg.useProgressive
    self.jpegUseOptimizeScanSwitch.isOn = viewModel.jpeg.useOptimizeScan
    self.jpegUseDcScanOptionSwitch.isOn = viewModel.jpeg.useCompatibleDCScanOption
    self.jpegUsePsnrQuantTableSwitch.isOn = viewModel.jpeg.usePSNRQuantTable
    
    self.pngUseInterlacingSwitch.isOn = viewModel.png.useInterlacing
    self.pngCompressionLevelButton.setTitle(viewModel.png.compressionLevel.title, for: .normal)

    self.webpMethodButton.setTitle(viewModel.webp.method.title, for: .normal)
    self.webpImageHintButton.setTitle(viewModel.webp.imageHint.title, for: .normal)
  }
}
