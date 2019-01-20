// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

/// Protocol representing all cases an alert can have.
protocol AlertOptions {
  var title: String { get }
  var isAvailable: Bool { get }

  static var allValues: [Self] { get }
}

extension AlertOptions {
  var isAvailable: Bool {
    return true
  }
}

/// Representation of an alert content.
struct AlertContent<Options: AlertOptions> {
  let title: String
  let message: String?
  let optionsType: Options.Type
}

/// Binding of an alert content and its action.
struct Alert<Options: AlertOptions> {
  let content: AlertContent<Options>
  let actionHandler: (Options) -> Void
}

extension UIViewController {
  func presentAlert<Options>(_ alertContent: AlertContent<Options>, from sourceView: UIView, using handler: @escaping (Options) -> Void) {
    let alertController = UIAlertController(alert: Alert(content: alertContent, actionHandler: handler))
    alertController.popoverPresentationController?.sourceView = sourceView
    alertController.popoverPresentationController?.sourceRect = CGRect(x: sourceView.bounds.width - 50,
                                                                       y: sourceView.bounds.origin.x,
                                                                       width: 50,
                                                                       height: sourceView.bounds.height)
    self.present(alertController, animated: true, completion: nil)
  }

  func presentError(_ error: Swift.Error) {
    let alertController = UIAlertController(title: NSLocalizedString("Error", comment: "Error alert title"),
                                            message: extractErrorMessage(from: error),
                                            preferredStyle: .alert)
    alertController.addAction(UIAlertAction(title: NSLocalizedString("Ok", comment: "Ok button"), style: .default, handler: nil))
    self.present(alertController, animated: true, completion: nil)
  }
}

extension UIAlertController {
  convenience init<Options>(alert: Alert<Options>) {
    self.init(title: alert.content.title, message: alert.content.message, preferredStyle: .actionSheet)

    alert.content.optionsType.allValues
      .filter { $0.isAvailable }
      .map { option in
        UIAlertAction(title: option.title, style: .default, handler: { _ in
          alert.actionHandler(option)
        })
      }
      .forEach(self.addAction)

    self.addAction(UIAlertAction(title: NSLocalizedString("Cancel", comment: "Cancel alert button"), style: .cancel, handler: nil))
  }
}

private func extractErrorMessage(from error: Swift.Error) -> String {
  let nsError = error as NSError
  let name: String = {
    if let name = nsError.userInfo[FSPErrorNameKey] as? String {
      return name
    } else {
      return NSLocalizedString("Unknown", comment: "Unknown error alert message")
    }
  }()

  let messageFormat = NSLocalizedString("Operation failed: %@", comment: "Error alert message format")
  return String.localizedStringWithFormat(messageFormat, name)
}

/// Type-erased coordinator to present an AlertContent and assign the picked value to a property
struct AnyAlertOptionController {
  // MARK: - Private Properties

  private let presentClosure: (UIViewController, UIView) -> Void

  // MARK: - Public Methods

  init<Options, Destination>(alertContent: AlertContent<Options>, destination: Destination, keyPath: ReferenceWritableKeyPath<Destination, Options>) {
    self.presentClosure = { viewController, sourceView in
      viewController.presentAlert(alertContent, from: sourceView, using: { option in
        destination[keyPath: keyPath] = option
      })
    }
  }

  func presentAlert(from viewController: UIViewController, sourceView: UIView) {
    self.presentClosure(viewController, sourceView)
  }
}

struct ValueHandler<Value> {
  // MARK: - Private Properties

  private let updateClosure: (Value) -> Void

  // MARK: - Public Methods

  init<Destination>(destination: Destination, keyPath: ReferenceWritableKeyPath<Destination, Value>) {
    self.updateClosure = { value in
      destination[keyPath: keyPath] = value
    }
  }

  func update(_ value: Value) {
    self.updateClosure(value)
  }
}

struct Alerts {
  static let imageSource = AlertContent(title: NSLocalizedString("Select source image source", comment: "Source image alert title"),
                                        message: nil,
                                        optionsType: ImagePickerCoordinator.Source.self)

  static let inputType = AlertContent(title: NSLocalizedString("How would should the source be loaded?", comment: "Source load method alert title"),
                                      message: nil,
                                      optionsType: IOType.self)

  static let outputType = AlertContent(title: NSLocalizedString("How would should the target be saved?", comment: "Target save method alert title"),
                                       message: nil,
                                       optionsType: IOType.self)

  static let inputFormat = AlertContent(title: NSLocalizedString("Select an image input format", comment: "Image input format alert title"),
                                        message: nil,
                                        optionsType: OutputImageFormat.self)

  static let outputFormat = AlertContent(title: NSLocalizedString("Select an image output format", comment: "Image output format alert title"),
                                         message: nil,
                                         optionsType: OutputImageFormat.self)

  static let encodingMode = AlertContent(title: NSLocalizedString("Select encoding mode", comment: "Image encoding mode title"),
                                         message: nil,
                                         optionsType: SpectrumViewModel.CompressionMode.self)

  static let rotation = AlertContent(title: NSLocalizedString("Select a rotation option", comment: "Image rotation format alert title"),
                                     message: nil,
                                     optionsType: SpectrumViewModel.ImageRotationOptions.self)

  static let scaling = AlertContent(title: NSLocalizedString("Select a scaling option", comment: "Image scaling format alert title"),
                                    message: nil,
                                    optionsType: SpectrumViewModel.ImageScalingOptions.self)

  static let cropping = AlertContent(title: NSLocalizedString("Select a cropping option", comment: "Image cropping format alert title"),
                                     message: nil,
                                     optionsType: SpectrumViewModel.ImageCroppingOptions.self)

  static let configurationGeneralDefaultBackgroundColor = AlertContent(title: NSLocalizedString("Select default background color", comment: "Configuration general background color title"),
                                                                       message: nil,
                                                                       optionsType: ConfigurationViewModel.DefaultBackgroundColor.self)
  
  static let configurationPngCompressionLevel = AlertContent(title: NSLocalizedString("Select PNG compression level", comment: "Configuration PNG compression level"),
                                                             message: nil,
                                                             optionsType: ConfigurationViewModel.CompressionLevel.self)

  static let configurationWebpMethod = AlertContent(title: NSLocalizedString("Select WebP method", comment: "Configuration webp method title"),
                                                    message: nil,
                                                    optionsType: ConfigurationViewModel.WebpMethod.self)

  static let configurationWebpImageHint = AlertContent(title: NSLocalizedString("Select WebP image hint", comment: "Configuration webp image hint title"),
                                                       message: nil,
                                                       optionsType: ConfigurationWebpImageHint.self)
}
