// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation
import XCTest

import SpectrumKitInstrumentationTestsHelpers

final class JpegJpegReEncodingTests: XCTestCase {
  // MARK: - Tests

  func test_whenEncodedAtHighQuality_thenComparisonScoreIsHigh() {
    self.transcodeAndAssertImage(at: .jpegRegular_128x85_Q75_baseLine,
                                 options: TranscodeOptions(encodeRequirement: .lossy95Jpeg),
                                 similarityRequirement: (path: .jpegRegular_128x85_Q75_baseLine, score: .greaterThan(0.99)))
  }

  func test_whenEncodedAtLowQuality_thenComparisonScoreIsNotThatHigh() {
    self.transcodeAndAssertImage(at: .jpegRegular_128x85_Q75_baseLine,
                                 options: TranscodeOptions(encodeRequirement: .lossy50Jpeg),
                                 similarityRequirement: (path: .jpegRegular_128x85_Q75_baseLine, score: .greaterThan(0.9)))
  }

  func test_whenEncodedAtExtremeLowQuality_thenComparisonScoreIsLow() {
    self.transcodeAndAssertImage(at: .jpegRegular_128x85_Q75_baseLine,
                                 options: TranscodeOptions(encodeRequirement: .lossy5Jpeg),
                                 similarityRequirement: (path: .jpegRegular_128x85_Q75_baseLine, score: .greaterThanWithOffset(0.65, offset: 0.1)))
  }

  func test_whenEncodedLosslessly_thenComparisonScoreIsHigh() {
    self.transcodeAndAssertImage(at: .jpegRegular_128x85_Q75_baseLine,
                                 options: TranscodeOptions(encodeRequirement: .losslessJpeg),
                                 similarityRequirement: (path: .jpegRegular_128x85_Q75_baseLine, score: .greaterThan(0.99)))
  }
}
