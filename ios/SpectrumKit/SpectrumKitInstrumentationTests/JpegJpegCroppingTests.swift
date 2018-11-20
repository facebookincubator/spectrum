// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation
import XCTest

import SpectrumKitInstrumentationTestsHelpers

final class JpegJpegCroppingTest: XCTestCase {
  // MARK: - Tests

  func test_whenAbsoluteCropping_thenSimilarToKnownResult() {
    let cropRequirement = AbsoluteToOriginCropRequirement(values: AbsoluteToOriginCropRequirementValues(top: 50, left: 100, bottom: 150, right: 300),
                                                          enforcement: .mustBeExact)
    self.transcodeCropAndAssertTestImage(with: cropRequirement,
                                         similarToImageAt: .jpegCropped_200x100_100_50_Q95)
  }

  func test_whenRelativeCropping_thenSimilarToKnownResult() {
    let cropRequirement = RelativeToOriginCropRequirement(values: RelativeToOriginCropRequirementValues(top: 0.2, left: 0.25, bottom: 0.8, right: 0.75),
                                                          enforcement: .mustBeExact)
    self.transcodeCropAndAssertTestImage(with: cropRequirement,
                                         similarToImageAt: .jpegCroppedRelative_50Px60P_center_Q95)
  }

  // MARK: - Private Methods

  private func transcodeCropAndAssertTestImage(with cropRequirement: CropRequirementProviding, similarToImageAt similarImagePath: TestDataPath) {
    let options = TranscodeOptions(encodeRequirement: .lossy95Jpeg)
    options.transformations.cropRequirement = cropRequirement
    self.transcodeAndAssertImage(at: .jpegRegular_800x530_Q75_progressive,
                                 options: options,
                                 similarityRequirement: (path: similarImagePath, score: .greaterThan(0.98)))
  }
}
