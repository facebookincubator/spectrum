// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation

enum OutputImageFormat: AlertOptions {
  case jpeg
  case png
  case gif
  case webp

  var imageFormat: EncodedImageFormat {
    switch self {
    case .jpeg:
      return EncodedImageFormat.jpeg
    case .png:
      return EncodedImageFormat.png
    case .gif:
      return EncodedImageFormat.gif
    case .webp:
      return EncodedImageFormat.webp
    }
  }

  // MARK: - AlertOptions

  var title: String {
    switch self {
    case .jpeg: return NSLocalizedString("JPEG", comment: "Image format option")
    case .png: return NSLocalizedString("PNG", comment: "Image format option")
    case .gif: return NSLocalizedString("GIF", comment: "Image format option")
    case .webp: return NSLocalizedString("WEBP", comment: "Image format option")
    }
  }

  static let allValues: [OutputImageFormat] = [.jpeg, .png, .gif, .webp]
}

extension IOType: AlertOptions {
  // MARK: - AlertOptions

  var title: String {
    switch self {
    case .file:
      return NSLocalizedString("File", comment: "Image source type option")
    case .bitmap:
      return NSLocalizedString("Bitmap", comment: "Image source type option")
    }
  }

  static let allValues: [IOType] = [.file, .bitmap]
}

extension SpectrumViewModel {
  enum ImageRotationOptions: AlertOptions {
    case none
    case clockwise90
    case clockwise180
    case clockwise270

    var value: Int {
      switch self {
      case .none: return 0
      case .clockwise90: return 90
      case .clockwise180: return 180
      case .clockwise270: return 270
      }
    }

    // MARK: - AlertOptions

    var title: String {
      switch self {
      case .none: return NSLocalizedString("None", comment: "Image rotation option")
      case .clockwise90: return NSLocalizedString("90\u{00B0} Clockwise", comment: "Image rotation option")
      case .clockwise180: return NSLocalizedString("180\u{00B0} Clockwise", comment: "Image rotation option")
      case .clockwise270: return NSLocalizedString("270\u{00B0} Clockwise", comment: "Image rotation option")
      }
    }

    static let allValues: [ImageRotationOptions] = [.none, .clockwise90, .clockwise180, .clockwise270]
  }

  enum CompressionMode: AlertOptions {
    case any
    case lossless
    case lossy

    // MARK: - AlertOptions

    var title: String {
      switch self {
      case .lossless: return NSLocalizedString("Lossless", comment: "Encode mode title")
      case .lossy: return NSLocalizedString("Lossy", comment: "Encode mode title")
      case .any: return NSLocalizedString("Any", comment: "Encode mode title")
      }
    }

    var mode: EncodeRequirementMode {
      switch self {
      case .any:
        return .any
      case .lossless:
        return .lossless
      case .lossy:
        return .lossy
      }
    }

    static let allValues: [CompressionMode] = [.any, .lossless, .lossy]
  }

  enum ImageScalingOptions: AlertOptions {
    case none
    case exact2x2
    case exact200x200
    case exact1000x1000
    case exactOrSmaller200x200
    case exactOrSmaller1000x1000

    private var size: CGSize? {
      switch self {
      case .none: return nil
      case .exact2x2: return CGSize(width: 2, height: 2)
      case .exact200x200, .exactOrSmaller200x200: return CGSize(width: 200, height: 200)
      case .exact1000x1000, .exactOrSmaller1000x1000: return CGSize(width: 1000, height: 1000)
      }
    }

    private var mode: ResizeRequirementMode {
      return self == .exactOrSmaller200x200 || self == .exactOrSmaller1000x1000 ? .exactOrSmaller : .exact
    }

    var requirement: ResizeRequirement? {
      return self.size.map { ResizeRequirement(mode: self.mode, targetSize: $0) }
    }

    // MARK: - AlertOptions

    var title: String {
      switch self {
      case .none: return NSLocalizedString("None", comment: "Image scaling option")
      case .exact2x2: return NSLocalizedString("== 2x2", comment: "Image scaling option")
      case .exact200x200: return NSLocalizedString("== 200x200", comment: "Image scaling option")
      case .exact1000x1000: return NSLocalizedString("== 1000x1000", comment: "Image scaling option")
      case .exactOrSmaller200x200: return NSLocalizedString("<= 200x200", comment: "Image scaling option")
      case .exactOrSmaller1000x1000: return NSLocalizedString("<= 1000x1000", comment: "Image scaling option")
      }
    }

    static let allValues: [ImageScalingOptions] = [.none, .exact2x2, .exact200x200, .exact1000x1000, .exactOrSmaller200x200, .exactOrSmaller1000x1000]
  }

  enum ImageCroppingOptions: AlertOptions {
    case none
    case absolute50x50x100x100
    case absolute0x0x300x50
    case relativeExactBottomRightHalf
    case relativeApproximateBottomRightHalf
    case relativeApproximateQuarterAllAround

    var requirement: CropRequirementProviding? {
      switch self {
      case .none:
        return nil
      case .absolute50x50x100x100:
        return AbsoluteToOriginCropRequirement(values: AbsoluteToOriginCropRequirementValues(top: 50, left: 50, bottom: 100, right: 100), enforcement: .mustBeExact)
      case .absolute0x0x300x50:
        return AbsoluteToOriginCropRequirement(values: AbsoluteToOriginCropRequirementValues(top: 0, left: 0, bottom: 50, right: 300), enforcement: .mustBeExact)
      case .relativeExactBottomRightHalf:
        return RelativeToOriginCropRequirement(values: RelativeToOriginCropRequirementValues(top: 0, left: 0, bottom: 0.5, right: 0.5), enforcement: .mustBeExact)
      case .relativeApproximateBottomRightHalf:
        return RelativeToOriginCropRequirement(values: RelativeToOriginCropRequirementValues(top: 0, left: 0, bottom: 0.5, right: 0.5), enforcement: .approximate)
      case .relativeApproximateQuarterAllAround:
        return RelativeToOriginCropRequirement(values: RelativeToOriginCropRequirementValues(top: 0.25, left: 0.25, bottom: 0.75, right: 0.75), enforcement: .approximate)
      }
    }

    // MARK: - AlertOptions

    var title: String {
      switch self {
      case .none: return NSLocalizedString("None", comment: "Image cropping options")
      case .absolute50x50x100x100: return NSLocalizedString("Absolute (50, 50, 100, 100)", comment: "Image cropping option")
      case .absolute0x0x300x50: return NSLocalizedString("Absolute (0, 0, 300, 100)", comment: "Image cropping option")
      case .relativeExactBottomRightHalf: return NSLocalizedString("Relative (0, 0, 0.5, 0.5)", comment: "Image cropping option")
      case .relativeApproximateBottomRightHalf: return NSLocalizedString("Relative ~(0, 0, 0.5, 0.5)", comment: "Image cropping option")
      case .relativeApproximateQuarterAllAround: return NSLocalizedString("Relative ~(0.25, 0.25, 0.75, 0.75)", comment: "Image cropping option")
      }
    }

    static let allValues: [ImageCroppingOptions] = [.none, .absolute0x0x300x50, .absolute50x50x100x100, .relativeExactBottomRightHalf, .relativeApproximateBottomRightHalf, .relativeApproximateQuarterAllAround]
  }
}
