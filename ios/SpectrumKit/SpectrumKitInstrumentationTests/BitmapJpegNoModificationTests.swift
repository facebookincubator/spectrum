// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation
import XCTest

import SpectrumKitInstrumentationTestsHelpers

final class BitmapJpegNoModificationTests: XCTestCase {
  // MARK: - Tests

  func test_whenRgbEncoded_thenSimilarToExpected() {
    self.encodeBitmapAndAssertImage(at: .jpegRegular_800x530_Q75_progressive)
  }

  func test_whenGrayEncoded_thenSimilarToExpected() {
    self.encodeBitmapAndAssertImage(at: .jpegRegular_128x85_Q75_grayscale)
  }

  func test_whenArgbEncoded_thenSimilarToExpected() {
    self.encodeBitmapAndAssertImage(at: .pngRegular_128x85_ARGB)
  }

  // MARK: - Private Methods

  private func encodeBitmapAndAssertImage(at imagePath: TestDataPath) {
    self.encodeAndAssertImage(at: imagePath,
                              options: EncodeOptions(encodeRequirement: .lossy95Jpeg),
                              similarityRequirement: (path: imagePath, score: .greaterThan(0.99)))
  }
}
