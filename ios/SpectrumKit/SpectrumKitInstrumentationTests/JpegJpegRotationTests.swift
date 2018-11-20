// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation
import XCTest

import SpectrumKitInstrumentationTestsHelpers

final class JpegJpegRotationTests: XCTestCase {
  // MARK: - Tests

  func test_whenRotated90_thenVerySimilarTo90RotatedVersion() {
    self.transcodeRotateAndAssertImage(at: .jpegRegular_128x85_Q75_progressive, degrees: 90)
  }

  func test_whenRotated180_thenVerySimilarTo180RotatedVersion() {
    self.transcodeRotateAndAssertImage(at: .jpegRegular_128x85_Q75_progressive, degrees: 180)
  }

  func test_whenRotated270_thenVerySimilarTo270RotatedVersion() {
    self.transcodeRotateAndAssertImage(at: .jpegRegular_128x85_Q75_progressive, degrees: 270)
  }

  func test_whenRotated90WithExifOrientation6_thenVerySimilarTo90RotatedVersion() {
    self.transcodeRotateAndAssertImage(at: .jpegRegular_128x85_Q75_EXIF_orientation_6_baseLine, degrees: 90)
  }

  // MARK: - Private Methods

  private func transcodeRotateAndAssertImage(at imagePath: TestDataPath, degrees: Int) {
    let options = TranscodeOptions(encodeRequirement: .lossy95Jpeg)
    options.transformations.rotateRequirement = RotateRequirement(degrees: degrees)
    self.transcodeAndAssertImage(at: imagePath,
                                 options: options,
                                 rotationDegrees: degrees,
                                 similarityRequirement: (path: imagePath, score: .greaterThan(0.98)))
  }
}
