// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation

public extension Data {
  public var imageFormat: ImageFormat? {
    switch self.first {
    case 0xFF?:
      return EncodedImageFormat.jpeg
    case 0x89?:
      return EncodedImageFormat.png
    default:
      return nil
    }
  }
}
