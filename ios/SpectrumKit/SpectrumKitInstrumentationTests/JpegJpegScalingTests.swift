// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation
import XCTest

import SpectrumKitInstrumentationTestsHelpers

final class JpegJpegScalingTests: XCTestCase {
  // MARK: - Tests

  func test_whenScaledDown_thenSimilarToSmallerVersion() {
    self.transcodeScaleAndAssertImage(at: .jpegRegular_800x530_Q75_progressive,
                                      similarToImageAt: .jpegRegular_128x85_Q75_progressive)
  }

  func test_whenScaledDownGrayScale_thenSimilarToSmallerVersion() {
    self.transcodeScaleAndAssertImage(at: .jpegRegular_800x530_Q75_grayscale,
                                      similarToImageAt: .jpegRegular_128x85_Q75_grayscale)
  }

  // MARK: - Private Methods

  private func transcodeScaleAndAssertImage(at imagePath: TestDataPath,
                                            similarToImageAt similarImagePath: TestDataPath) {
    let options = TranscodeOptions(encodeRequirement: .lossy95Jpeg)
    options.transformations.resizeRequirement = ResizeRequirement(mode: .exact,
                                                                  targetSize: CGSize(width: 128, height: 128))

    self.transcodeAndAssertImage(at: imagePath,
                                 options: options,
                                 similarityRequirement: (path: similarImagePath, score: .greaterThan(0.92)))
  }
}
