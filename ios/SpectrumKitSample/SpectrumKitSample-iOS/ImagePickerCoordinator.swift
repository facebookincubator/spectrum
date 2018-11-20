// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

protocol ImagePickerCoordinatorDelegate: class {
  func imagePickerCoordinator(_ imagePickerCoordinator: ImagePickerCoordinator, didPickMedia source: ImageSource)
  func imagePickerCoordinator(_ imagePickerCoordinator: ImagePickerCoordinator, didFailToPickImageWith error: Swift.Error)
}

final class ImagePickerCoordinator: NSObject, UINavigationControllerDelegate, UIImagePickerControllerDelegate {
  // MARK: - Types

  enum Source: AlertOptions {
    case camera
    case library

    fileprivate var imagePickerSource: UIImagePickerController.SourceType {
      switch self {
      case .camera: return .camera
      case .library: return .photoLibrary
      }
    }

    // MARK: - AlertOptions

    var title: String {
      switch self {
      case .camera: return NSLocalizedString("Camera", comment: "Image source - camera")
      case .library: return NSLocalizedString("Library", comment: "Image source - library")
      }
    }

    var isAvailable: Bool {
      return UIImagePickerController.isSourceTypeAvailable(self.imagePickerSource)
    }

    static let allValues: [Source] = [.camera, .library]
  }

  // MARK: - Public Properties

  weak var viewController: UIViewController?
  weak var delegate: ImagePickerCoordinatorDelegate?

  // MARK: - Public Methods

  func presentImageSourcePickerIfNeeded(from sourceView: UIView) {
    let availableSources = Source.allValues.filter { $0.isAvailable }
    if let source = availableSources.first, availableSources.count == 1 {
      self.presentPicker(for: source)
    } else {
      self.viewController?.presentAlert(Alerts.imageSource, from: sourceView, using: self.presentPicker)
    }
  }

  // MARK: - UIImagePickerControllerDelegate

  func imagePickerControllerDidCancel(_: UIImagePickerController) {
    self.viewController?.dismiss(animated: true, completion: nil)
  }

  func imagePickerController(_: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey: Any]) {
    self.viewController?.dismiss(animated: true, completion: nil)

    do {
      self.delegate?.imagePickerCoordinator(self, didPickMedia: try ImageSource(info: info))
    } catch {
      self.delegate?.imagePickerCoordinator(self, didFailToPickImageWith: error)
    }
  }

  // MARK: - Private Properties

  private func presentPicker(for source: Source) {
    precondition(source.isAvailable, "Expecting to present \(source) but it's not available")

    let imagePickerViewController = UIImagePickerController()
    imagePickerViewController.sourceType = source.imagePickerSource
    imagePickerViewController.delegate = self
    imagePickerViewController.modalPresentationStyle = .formSheet
    self.viewController?.present(imagePickerViewController, animated: true, completion: nil)
  }
}
