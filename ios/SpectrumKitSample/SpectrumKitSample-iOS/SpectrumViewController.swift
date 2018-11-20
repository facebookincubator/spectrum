// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

final class SprectrumViewController: UIViewController, SpectrumViewModelDelegate, ImagePickerCoordinatorDelegate, ImageDifferenceViewControllerDelegate {
  // MARK: - Private Types

  private enum Segue: String {
    case showImageDifference
    case showConfiguration
  }

  // MARK: - Private Properties

  private let viewModel = SpectrumViewModel()
  private let imagePickerCoordinator = ImagePickerCoordinator()
  private var alertOptionControllerByButton = [UIButton: AnyAlertOptionController]()

  private var mView: SprectrumView {
    return self.view as! SprectrumView
  }

  // MARK: IBOutlets

  @IBOutlet private var sourceImageButton: UIButton!
  @IBOutlet private var inputTypeButton: UIButton!
  @IBOutlet private var outputTypeButton: UIButton!
  @IBOutlet private var outputFormatButton: UIButton!
  @IBOutlet private var compressionModeButton: UIButton!
  @IBOutlet private var rotationButton: UIButton!
  @IBOutlet private var scalingButton: UIButton!
  @IBOutlet private var croppingButton: UIButton!

  // MARK: - NSObject

  required init?(coder aDecoder: NSCoder) {
    super.init(coder: aDecoder)

    self.viewModel.delegate = self
    self.imagePickerCoordinator.delegate = self
    self.imagePickerCoordinator.viewController = self
  }

  // MARK: - UIViewController

  override init(nibName _: String?, bundle _: Bundle?) {
    fatalError("init(nibName:bundle:) not implemented")
  }

  override func viewDidLoad() {
    super.viewDidLoad()

    self.mView.viewModel = self.viewModel
    self.alertOptionControllerByButton = [
      self.inputTypeButton: AnyAlertOptionController(alertContent: Alerts.inputType, destination: self.viewModel, keyPath: \.inputType),
      self.outputTypeButton: AnyAlertOptionController(alertContent: Alerts.outputType, destination: self.viewModel, keyPath: \.outputType),
      self.outputFormatButton: AnyAlertOptionController(alertContent: Alerts.outputFormat, destination: self.viewModel, keyPath: \.outputFormat),
      self.compressionModeButton: AnyAlertOptionController(alertContent: Alerts.encodingMode, destination: self.viewModel, keyPath: \.compressionMode),
      self.rotationButton: AnyAlertOptionController(alertContent: Alerts.rotation, destination: self.viewModel, keyPath: \.rotation),
      self.scalingButton: AnyAlertOptionController(alertContent: Alerts.scaling, destination: self.viewModel, keyPath: \.scaling),
      self.croppingButton: AnyAlertOptionController(alertContent: Alerts.cropping, destination: self.viewModel, keyPath: \.cropping),
    ]
  }

  override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
    super.prepare(for: segue, sender: sender)

    switch segue.identifier.flatMap(Segue.init(rawValue:)) {
    case .showImageDifference?:
      guard
        let viewController = segue.destination as? ImageDifferenceViewController,
        let viewModel = sender as? ImageDifferenceViewModel
      else {
        fatalError("Unexpected viewController or viewModel for: \(String(describing: segue.identifier))")
      }

      viewController.viewModel = viewModel
      viewController.delegate = self

    case .showConfiguration?:
      guard let viewController = segue.destination as? ConfigurationViewController else {
        fatalError("Unexpected viewController for: \(String(describing: segue.identifier))")
      }

      viewController.viewModel = self.viewModel.makeConfigurationViewModel()
    case nil:
      fatalError("Unexpected segue: \(String(describing: segue.identifier))")
    }
  }

  // MARK: - ViewModelDelegate

  func viewModelDidUpdate(_ viewModel: SpectrumViewModel) {
    self.mView.updateView(with: viewModel)
  }

  func viewModelDidFinishTranscoding(_ viewModel: SpectrumViewModel) {
    do {
      let viewModel = try self.viewModel.makeImageDifferenceViewModel()
      self.performSegue(withIdentifier: Segue.showImageDifference.rawValue, sender: viewModel)
    } catch {
      self.presentError(error)
    }
  }

  func viewModel(_: SpectrumViewModel, didFailTranscodingWith error: Swift.Error) {
    self.presentError(error)
  }

  // MARK: - ImagePickerCoordinatorDelegate

  func imagePickerCoordinator(_: ImagePickerCoordinator, didPickMedia source: ImageSource) {
    self.viewModel.load(sourceImage: source)
  }

  func imagePickerCoordinator(_: ImagePickerCoordinator, didFailToPickImageWith error: Swift.Error) {
    self.presentError(error)
  }

  // MARK: - ImageDifferenceViewControllerDelegate

  func imageDifferenceViewControllerDidTapCloseButton(_: ImageDifferenceViewController) {
    self.dismiss(animated: true, completion: nil)
  }

  // MARK: - Private Methods

  // MARK: IBActions

  @IBAction private func didTapImageOptionsButton(_ sender: UIButton) {
    self.alertOptionControllerByButton[sender]!.presentAlert(from: self, sourceView: sender)
  }

  @IBAction private func didTapInputImageButton(_ sender: UIButton) {
    self.imagePickerCoordinator.presentImageSourcePickerIfNeeded(from: sender)
  }
}
