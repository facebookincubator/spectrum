// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation
import XCTest

import SpectrumKitInstrumentationTestsHelpers

final class PngJpegReEncodingTests: XCTestCase {
  // MARK: - Tests

  func test_whenPixelSpecificationRgb_thenSucceed() {
    self.transcodeAndAssertImage(at: .pngRegular_128x85_Q75,
                                 options: TranscodeOptions(encodeRequirement: .lossy95Jpeg),
                                 similarityRequirement: (path: .jpegRegular_128x85_Q75_baseLine, score: .greaterThan(0.99)))
  }

  func test_whenPixelSpecificationArgb_thenSucceed() {
    self.transcodeAndAssertImage(at: .pngRegular_128x85_ARGB,
                                 options: TranscodeOptions(encodeRequirement: .lossy95Jpeg),
                                 similarityRequirement: (path: .jpegRegular_128x85_Q75_baseLine, score: .greaterThan(0.99)))
  }

  func test_whenPixelSpecificationArgb_thenTransparentIsMappedToWhite() {
    self.transcodeAndAssertImage(at: .pngTransparent_16x16_ARGB,
                                 options: TranscodeOptions(encodeRequirement: .lossy95Jpeg),
                                 similarityRequirement: (path: .jpegRegular_16x16_white_Q75, score: .greaterThan(0.99)))
  }
}
