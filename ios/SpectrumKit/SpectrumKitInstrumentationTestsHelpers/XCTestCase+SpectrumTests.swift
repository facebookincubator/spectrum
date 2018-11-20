// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation
import XCTest

public typealias SimilarityRequirement = (path: TestDataPath, score: SimilarityScore)

public enum SimilarityScore {
  case greaterThan(CGFloat)
  case greaterThanWithOffset(CGFloat, offset: CGFloat)

  fileprivate func isWithinScore(_ value: CGFloat) -> Bool {
    switch self {
    case let .greaterThan(score):
      return (score ... 1).contains(value)
    case let .greaterThanWithOffset(score, offset):
      return (score ... score + offset).contains(value)
    }
  }

  fileprivate var isValid: Bool {
    switch self {
    case let .greaterThan(score):
      return score < 1
    case let .greaterThanWithOffset(score, offset):
      return score + offset < 1
    }
  }
}

enum SpectrumTestError: LocalizedError {
  case noData

  var errorDescription: String? {
    return NSLocalizedString("No data after transcode", comment: "Error in test case")
  }
}

public extension XCTestCase {
  // MARK: - Public Methods

  public func encodeAndAssertImage(at imageDataPath: TestDataPath,
                                   options: EncodeOptions,
                                   rotationDegrees: Int = 0,
                                   similarityRequirement: SimilarityRequirement? = nil) {
    do {
      let fromImage = try imageDataPath.image(forResourceInBundleOfObject: self)

      var error: NSError?
      let result = Spectrum.shared.encodeImage(fromImage, options: options, error: &error)

      guard let toImageData = result.data, let toImage = UIImage(data: toImageData) else {
        throw error ?? SpectrumTestError.noData
      }

      self.assertFinalImageSize(with: options.transformations, fromImageSize: fromImage.size, toImageSize: toImage.size)

      try self.assertSimilarityScore(fromImage: fromImage,
                                     toImageData: toImageData,
                                     options: options,
                                     rotationDegrees: rotationDegrees,
                                     similarityRequirement: similarityRequirement)
    } catch {
      XCTFail("Error while encoding: \(error)")
    }
  }

  public func transcodeAndAssertImage(at imageDataPath: TestDataPath,
                                      options: TranscodeOptions,
                                      rotationDegrees: Int = 0,
                                      similarityRequirement: SimilarityRequirement? = nil) {
    do {
      try self.internalTranscodeAndAssertImage(at: imageDataPath,
                                               options: options,
                                               rotationDegrees: rotationDegrees,
                                               similarityRequirement: similarityRequirement)
    } catch {
      XCTFail("Error while transcoding: \(error)")
    }
  }

  // MARK: - Private Methods

  private func internalTranscodeAndAssertImage(at imageDataPath: TestDataPath,
                                               options: TranscodeOptions,
                                               rotationDegrees: Int = 0,
                                               similarityRequirement: SimilarityRequirement? = nil) throws {
    let fromImageUrl = try imageDataPath.url(forResourceInBundleOfObject: self)

    var error: NSError?
    let result = Spectrum.shared.transcodeImage(fromFileAt: fromImageUrl, options: options, error: &error)

    let fromImageData = try Data(contentsOf: fromImageUrl)
    guard let fromImage = UIImage(data: fromImageData) else {
      throw TestError.invalidImage
    }

    guard let toImageData = result.data, let toImage = UIImage(data: toImageData) else {
      throw error ?? SpectrumTestError.noData
    }

    self.assertEncodedImageData(options: options, fromImageData: fromImageData, toImageData: toImageData)
    self.assertFinalImageSize(with: options.transformations, fromImageSize: fromImage.size, toImageSize: toImage.size)

    try self.assertSimilarityScore(fromImage: fromImage,
                                   toImageData: toImageData,
                                   options: options,
                                   rotationDegrees: rotationDegrees,
                                   similarityRequirement: similarityRequirement)
  }

  private func assertSimilarityScore(fromImage: UIImage,
                                     toImageData: Data,
                                     options: Options,
                                     rotationDegrees: Int,
                                     similarityRequirement: SimilarityRequirement?) throws {
    guard let toImage = UIImage(data: toImageData, scale: fromImage.scale) else {
      throw TestError.invalidImage
    }

    if let similarityRequirement = similarityRequirement {
      let similarImage = try similarityRequirement.path.image(forResourceInBundleOfObject: self).rotated(by: rotationDegrees)
      XCTAssertTrue(similarityRequirement.score.isWithinScore(similarImage.similarityFactor(with: toImage)))
    }
  }

  private func assertEncodedImageData(options: Options, fromImageData: Data, toImageData: Data) {
    XCTAssertEqual(toImageData.imageFormat, options.outputImageFormat)

    let transformations = options.transformations
    if transformations.resizeRequirement == nil && transformations.cropRequirement == nil &&
      options.encodeRequirement?.mode != .lossy && transformations.rotateRequirement == nil &&
      options.outputImageFormat == fromImageData.imageFormat {
      XCTAssertEqual(fromImageData, toImageData)
    } else {
      XCTAssertNotEqual(fromImageData, toImageData)
    }
  }

  // MARK: Image Size

  private func assertFinalImageSize(with transformations: Transformations, fromImageSize: CGSize, toImageSize: CGSize) {
    switch (transformations.cropRequirement, transformations.resizeRequirement) {
    case let (cropRequirement?, nil):
      self.assertFinalImageSize(with: cropRequirement, fromImageSize: fromImageSize, toImageSize: toImageSize)
    case let (nil, resizeRequirement?):
      self.assertFinalImageSize(with: resizeRequirement, fromImageSize: fromImageSize, toImageSize: toImageSize)
    case (_?, _?):
      XCTFail("Size checks for cropRequirement & resizeRequirement not implemented yet!")
    case (nil, nil):
      XCTAssertEqual(toImageSize, fromImageSize.rotated(by: transformations.rotateRequirement?.degrees ?? 0))
    }
  }

  private func assertFinalImageSize(with cropRequirement: CropRequirementProviding, fromImageSize: CGSize, toImageSize: CGSize) {
    let targetSize = cropRequirement.targetSize(for: fromImageSize)
    switch cropRequirement.enforcement {
    case .approximate:
      XCTAssertLessThanOrEqual(toImageSize.width, targetSize.width)
      XCTAssertLessThanOrEqual(toImageSize.height, targetSize.height)
    case .mustBeExact:
      XCTAssertEqual(toImageSize, targetSize)
    }
  }

  private func assertFinalImageSize(with resizeRequirement: ResizeRequirement, fromImageSize: CGSize, toImageSize: CGSize) {
    switch resizeRequirement.mode {
    case .exact:
      XCTAssertEqual(toImageSize, fromImageSize.scaledDown(fittingIn: resizeRequirement.targetSize))
    case .exactOrSmaller:
      XCTAssertLessThanOrEqual(toImageSize.width, resizeRequirement.targetSize.width)
      XCTAssertLessThanOrEqual(toImageSize.height, resizeRequirement.targetSize.height)
    default:
      XCTFail("Size checks for custom resize requirements not implemented yet!")
    }
  }
}

enum TestError: Error {
  case fileNotFound
  case invalidImage
}

private extension CropRequirementProviding {
  func targetSize(for imageSize: CGSize) -> CGSize {
    switch self {
    case let cropRequirement as AbsoluteToOriginCropRequirement:
      return CGSize(width: CGFloat(cropRequirement.values.right - cropRequirement.values.left),
                    height: CGFloat(cropRequirement.values.bottom - cropRequirement.values.top))
    case let cropRequirement as RelativeToOriginCropRequirement:
      return CGSize(width: round(CGFloat(cropRequirement.values.right - cropRequirement.values.left) * imageSize.width),
                    height: round(CGFloat(cropRequirement.values.bottom - cropRequirement.values.top) * imageSize.height))
    default:
      fatalError("Unexpected cropping requirement")
    }
  }
}
