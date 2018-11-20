// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation

public struct TestDataPath {
  // MARK: - Private Properties

  let name: String

  // MARK: - Public Methods

  func image(forResourceInBundleOfObject object: AnyObject) throws -> UIImage {
    let path = try self.url(forResourceInBundleOfObject: object).path
    return try UIImage(contentsOfFile: path).valueOrThrow(TestError.fileNotFound)
  }

  func url(forResourceInBundleOfObject object: AnyObject) throws -> URL {
    return try Bundle(for: type(of: object))
      .url(forResource: self.name, withExtension: nil)
      .valueOrThrow(TestError.fileNotFound)
  }

  // MARK: - Static Properties

  public static let jpegRegular_128x85_Q75_baseLine = TestDataPath(name: "128x85_q75_baseline.jpg")
  public static let jpegRegular_128x85_Q75_EXIF_orientation_6_baseLine = TestDataPath(name: "128x85_q75_exif_orientation_6_baseline.jpg")
  public static let jpegRegular_128x85_Q75_grayscale = TestDataPath(name: "128x85_q75_grayscale.jpg")
  public static let jpegRegular_128x85_Q75_progressive = TestDataPath(name: "128x85_q75_progressive.jpg")
  public static let jpegRegular_800x530_Q75_baseline = TestDataPath(name: "800x530_q75_baseline.jpg")
  public static let jpegRegular_800x530_Q75_progressive = TestDataPath(name: "800x530_q75_progressive.jpg")
  public static let jpegRegular_800x530_Q75_grayscale = TestDataPath(name: "800x530_q75_grayscale.jpg")
  public static let jpegRegular_4096x2713_benchmarkLarge = TestDataPath(name: "4096x2713_benchmark_large.jpg")
  public static let jpegRegular_16x16_white_Q75 = TestDataPath(name: "16x16_white_q75.jpg")
  public static let jpegRegular_16x16_black_Q75 = TestDataPath(name: "16x16_black_q75.jpg")
  public static let jpegRegular_16x16_white_Q75_grayscale = TestDataPath(name: "16x16_white_q75_grayscale.jpg")
  public static let jpegRegular_16x16_black_Q75_grayscale = TestDataPath(name: "16x16_black_q75_grayscale.jpg")
  public static let jpegRegular_16x16_cABCDF0_Q75 = TestDataPath(name: "16x16_cABCDF0_q75.jpg")
  public static let jpegCropped_200x100_100_50_Q95 = TestDataPath(name: "c200x100_o100_50_q95.jpg")
  public static let jpegCroppedRelative_50Px60P_center_Q95 = TestDataPath(name: "c50px60p_center_q95.jpg")

  public static let pngRegular_128x85_Q75 = TestDataPath(name: "128x85.png")
  public static let pngRegular_128x85_ARGB = TestDataPath(name: "128x85_argb.png")
  public static let pngRegular_800x530_Q75 = TestDataPath(name: "800x530.png")
  public static let pngTransparent_16x16_ARGB = TestDataPath(name: "16x16_transparent_argb.png")
}
