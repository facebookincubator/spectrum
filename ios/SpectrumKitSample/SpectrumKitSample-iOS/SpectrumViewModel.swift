// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Photos
import UIKit.UIImage

protocol SpectrumViewModelDelegate: class {
  func viewModelDidUpdate(_ viewModel: SpectrumViewModel)
  func viewModel(_ viewModel: SpectrumViewModel, didFailTranscodingWith error: Swift.Error)
  func viewModelDidFinishTranscoding(_ viewModel: SpectrumViewModel)
}

final class SpectrumViewModel: NSObject {
  // MARK: - Public Properties

  weak var delegate: SpectrumViewModelDelegate?

  private(set) var source = ImageSource(initialImage: ()) {
    didSet {
      guard self.source != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  var sourceImageDescription: String? {
    return "Size: \(self.source.image.size) @\(self.source.image.scale)x"
  }

  var inputType: IOType = .file {
    didSet {
      guard self.inputType != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  var outputType: IOType = .file {
    didSet {
      guard self.outputType != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  var outputFormat: OutputImageFormat = .jpeg {
    didSet {
      guard self.outputFormat != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  var compressionMode: CompressionMode = .any {
    didSet {
      guard self.compressionMode != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  var actionButtonTitle: String {
    switch (self.inputType, self.outputType) {
    case (.file, .file):
      return NSLocalizedString("Transcode", comment: "Launch operation call to action button title")
    case (.file, .bitmap):
      return NSLocalizedString("Decode", comment: "Launch operation call to action button title")
    case (.bitmap, .file):
      return NSLocalizedString("Encode", comment: "Launch operation call to action button title")
    case (.bitmap, .bitmap):
      return NSLocalizedString("Transform", comment: "Launch operation call to action button title")
    }
  }

  var showsQuality: Bool {
    return self.outputType == .file && self.compressionMode != .lossless
  }

  var showsEncodingMode: Bool {
    return self.outputType == .file
  }

  var showsOutputFormat: Bool {
    return self.outputType == .file
  }

  var mustTranscode: Bool = true {
    didSet {
      guard self.mustTranscode != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  var quality: Int = 80 {
    didSet {
      guard self.quality != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  static let qualityPickerOptions = (EncodeRequirementQualityMin ... EncodeRequirementQualityMax / 5).map { $0 * 5 }

  var rotation: ImageRotationOptions = .none {
    didSet {
      guard self.rotation != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  var scaling: ImageScalingOptions = .none {
    didSet {
      guard self.scaling != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  var cropping: ImageCroppingOptions = .none {
    didSet {
      guard self.cropping != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  private(set) var isTranscoding = false {
    didSet {
      guard self.isTranscoding != oldValue else { return }
      self.delegate?.viewModelDidUpdate(self)
    }
  }

  var isTranscodeButtonEnabled: Bool {
    return self.isTranscoding == false
  }

  let configuration = Configuration()

  // MARK: - Private Properties

  private var transcodedImageRepresentation: ImageRepresentation?

  // MARK: - Public Methods

  func load(sourceImage: ImageSource) {
    self.transcodedImageRepresentation = nil
    self.source = sourceImage
  }

  func transcode() {
    self.isTranscoding = true
    SpectrumController.transcode(options: self.makeOptions(),
                                 source: self.source,
                                 inputType: self.inputType,
                                 outputType: self.outputType,
                                 completion: { [weak self] result in
                                   guard let strongSelf = self else { return }
                                   strongSelf.isTranscoding = false

                                   switch result {
                                   case let .success(imageRepresentation):
                                     strongSelf.transcodedImageRepresentation = imageRepresentation
                                     strongSelf.delegate?.viewModelDidFinishTranscoding(strongSelf)
                                   case let .error(error):
                                     strongSelf.delegate?.viewModel(strongSelf, didFailTranscodingWith: error)
                                   }
    })
  }

  func makeConfigurationViewModel() -> ConfigurationViewModel {
    return ConfigurationViewModel(configuration: self.configuration)
  }

  func makeImageDifferenceViewModel() throws -> ImageDifferenceViewModel {
    switch self.transcodedImageRepresentation {
    case let .bytes(data)?:
      return try ImageDifferenceViewModel(sourceImage: self.source.image,
                                          transcodedImageData: data,
                                          imageFormat: self.outputFormat)
    case .bitmap(_)?, nil:
      throw Error.unsupportedBitmapOutput
    }
  }

  // MARK: - Private Methods

  private func makeOptions() -> SpectrumController.Options {
    return SpectrumController.Options(encodeRequirement: self.makeEncodeRequirement(),
                                      rotateRequirement: self.makeRotateRequirement(),
                                      cropRequirement: self.cropping.requirement,
                                      resizeRequirement: self.scaling.requirement,
                                      outputImagePixelSpecification: self.makeOutputImagePixelSpecification(),
                                      configuration: self.configuration,
                                      metadata: nil)
  }

  private func makeRotateRequirement() -> RotateRequirement? {
    guard self.rotation.value != 0 else { return nil }
    return RotateRequirement(degrees: self.rotation.value)
  }

  private func makeEncodeRequirement() -> EncodeRequirement? {
    guard self.outputType != .bitmap else { return nil }
    return EncodeRequirement(format: self.outputFormat.imageFormat,
                             mode: self.compressionMode.mode,
                             quality: self.quality)
  }

  private func makeOutputImagePixelSpecification() -> ImagePixelSpecification? {
    return self.outputType == .bitmap ? .rgba : nil
  }
}
