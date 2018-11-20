// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

final class HairlineConstraint: NSLayoutConstraint {
  override var constant: CGFloat {
    get {
      return 1 / UIScreen.main.scale
    }
    set {}
  }
}

extension UIColor {
  static let tintColor = UIColor(red: 185 / 255, green: 70 / 255, blue: 154 / 255, alpha: 1)
}

enum ImageRepresentation {
  case bytes(Data)
  case bitmap(UIImage)
}

enum Result<T> {
  case success(T)
  case error(Swift.Error)
}

enum Error: LocalizedError {
  case unknown
  case noImageInInfo
  case unsupportedBitmapOutput
  case noUrlForImage
  case noDataInResult

  var errorDescription: String? {
    switch self {
    case .noDataInResult:
      return NSLocalizedString("Operation generated no data.", comment: "Failed operation")
    case .unsupportedBitmapOutput:
      return NSLocalizedString("Unsupported bitmap output.", comment: "Decode operation not supported yet")
    case .unknown:
      return NSLocalizedString("Unknown.", comment: "Unknown")
    case .noUrlForImage:
      return NSLocalizedString("No url for image.", comment: "Cannot open image")
    case .noImageInInfo:
      return NSLocalizedString("No image in info dictionary.", comment: "Cannot open image")
    }
  }
}

struct ImageSource: Equatable {
  private(set) var image: UIImage
  private(set) var url: URL?

  init(initialImage: Void) {
    guard
      let initialImageUrl = Bundle.main.url(forResource: "panda", withExtension: "jpg"),
      let initialImage = UIImage(contentsOfFile: initialImageUrl.path) else {
      fatalError("Unable to find initial image")
    }

    self.image = initialImage
    self.url = initialImageUrl
  }

  init(info: [UIImagePickerController.InfoKey: Any]) throws {
    guard let image = info[.originalImage] as? UIImage else {
      throw Error.noImageInInfo
    }

    self.image = image
    if #available(iOS 11.0, *) {
      self.url = info[.imageURL] as? URL
    } else {
      self.url = nil
    }
  }
}

enum IOType {
  case file
  case bitmap
}
