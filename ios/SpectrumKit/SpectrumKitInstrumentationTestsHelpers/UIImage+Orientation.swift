// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit.UIImage

public extension UIImage {
  func transformToUpOrientation() -> UIImage {
    return self.rotated(by: 0)
  }

  /// Rotates the UIImage by `degrees` in addition to its orientation.
  ///
  /// - Parameter degrees: The degress to rotate the image in. Has to be a multiple of 90.
  /// - Returns: A new image rotated by degrees and its orientation. The retruned image will always
  ///            have a up orientation.
  func rotated(by degrees: Int) -> UIImage {
    guard degrees != 0 || self.imageOrientation != .up else {
      return self
    }

    guard let cgImage = self.cgImage else {
      fatalError("Failed to extract CGImage from UIImage")
    }

    let newSize = self.size.rotated(by: degrees)
    UIGraphicsBeginImageContextWithOptions(newSize, false, self.scale)

    defer {
      UIGraphicsEndImageContext()
    }

    guard let context = UIGraphicsGetCurrentContext() else {
      fatalError("Failed to get current image context")
    }

    context.translateBy(x: newSize.width / 2, y: newSize.height / 2)
    context.rotate(by: CGFloat(degrees + self.imageOrientation.degrees) * .pi / 180)

    context.scaleBy(x: self.imageOrientation.isMirrored.horizontally ? -1 : 1,
                    y: self.imageOrientation.isMirrored.vertically ? 1 : -1)

    let nonOrientationSpecificImageSize = self.size.rotated(by: -self.imageOrientation.degrees)
    context.draw(cgImage, in: CGRect(origin: CGPoint(x: -nonOrientationSpecificImageSize.width / 2,
                                                     y: -nonOrientationSpecificImageSize.height / 2),
                                     size: nonOrientationSpecificImageSize))

    guard let image = UIGraphicsGetImageFromCurrentImageContext() else {
      fatalError("Failed to extract UIImage from context")
    }

    return image
  }
}

private extension UIImage.Orientation {
  var degrees: Int {
    switch self {
    case .up, .upMirrored: return 0
    case .right, .rightMirrored: return 90
    case .down, .downMirrored: return 180
    case .left, .leftMirrored: return 270
    }
  }

  var isMirrored: (horizontally: Bool, vertically: Bool) {
    return (horizontally: [.upMirrored, .downMirrored].contains(self),
            vertically: [.upMirrored, .downMirrored].contains(self))
  }
}

extension CGSize {
  func rotated(by degrees: Int) -> CGSize {
    switch (degrees + 360) % 360 {
    case 0, 180:
      return self
    case 90, 270:
      return CGSize(width: self.height, height: self.width)
    default:
      fatalError("Degress has to be a multiple of 90")
    }
  }

  func scaledDown(fittingIn: CGSize) -> CGSize {
    let ratio = min(fittingIn.width / self.width, fittingIn.height / self.height)

    guard ratio < 1 else { return self }
    return CGSize(width: round(self.width * ratio), height: round(self.height * ratio))
  }
}
