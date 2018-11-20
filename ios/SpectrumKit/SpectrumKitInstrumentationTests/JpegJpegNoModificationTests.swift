// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation
import XCTest

import SpectrumKitInstrumentationTestsHelpers

final class JpegJpegNoModificationTests: XCTestCase {
  // MARK: - Tests

  func test_whenForcingLossLess_thenCopiedExactly() {
    self.transcodeNoModificationAndAssertImage(at: .jpegCropped_200x100_100_50_Q95,
                                               encodeRequirement: .lossy95Jpeg)
  }

  func test_whenSettingQualityLevel_thenTranscoded() {
    self.transcodeNoModificationAndAssertImage(at: .jpegCropped_200x100_100_50_Q95,
                                               encodeRequirement: .lossy95Jpeg)
  }

  func test_whenSettingQualityLevelAndNotAnyInputImageSpecification_thenTranscoded() {
    self.transcodeNoModificationAndAssertImage(at: .jpegCropped_200x100_100_50_Q95,
                                               encodeRequirement: .lossy95Jpeg)
  }

  func test_whenContainsExifOrientation_thenOrientationIsPreserved() {
    self.transcodeNoModificationAndAssertImage(at: .jpegRegular_128x85_Q75_EXIF_orientation_6_baseLine,
                                               encodeRequirement: .lossy95Jpeg)
  }

  // MARK: - Private Methods

  private func transcodeNoModificationAndAssertImage(at imagePath: TestDataPath,
                                                     encodeRequirement: EncodeRequirement) {
    self.transcodeAndAssertImage(at: imagePath, options: TranscodeOptions(encodeRequirement: encodeRequirement))
  }
}

private extension CGSize {
  static let testImageSize = CGSize(width: 200, height: 100)
}

private extension ImageSpecification {
  static var testImage: ImageSpecification {
    return ImageSpecification(size: .testImageSize,
                              format: EncodedImageFormat.jpeg,
                              pixelSpecification: .rgb,
                              orientation: .up,
                              chromaSamplingMode: .mode444,
                              metadata: nil)
  }
}
