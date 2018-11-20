// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation
import UIKit.UIImage

protocol ImageDifferenceViewModelDelegate: class {
  func imageDifferenceViewModelDidChange(_ viewModel: ImageDifferenceViewModel)
}

final class ImageDifferenceViewModel {
  enum Error: LocalizedError {
    case failedToConvertToImage

    var errorDescription: String? {
      return NSLocalizedString("Error while converting the data back to an image", comment: "Unknown conversion error description")
    }
  }

  // MARK: - Public Properties

  let sourceImage: UIImage
  let transcodedImage: UIImage

  var ratio: Float = 1 {
    didSet {
      guard self.ratio != oldValue else { return }
      self.delegate?.imageDifferenceViewModelDidChange(self)
    }
  }

  weak var delegate: ImageDifferenceViewModelDelegate?

  var transcodedImageViewAlpha: CGFloat {
    return CGFloat(self.ratio)
  }

  // MARK: - Public Methods

  init(sourceImage: UIImage, transcodedImageData: Data, imageFormat _: OutputImageFormat) throws {
    self.sourceImage = sourceImage

    if let image = UIImage(data: transcodedImageData) {
      self.transcodedImage = image
    } else {
      throw Error.failedToConvertToImage
    }
  }
}
