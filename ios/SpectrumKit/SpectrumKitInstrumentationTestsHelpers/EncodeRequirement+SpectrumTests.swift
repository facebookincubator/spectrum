// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation

public extension EncodeRequirement {
  public static var lossy95Jpeg: EncodeRequirement {
    return EncodeRequirement(format: .jpeg, mode: .lossy, quality: 95)
  }

  public static var lossy50Jpeg: EncodeRequirement {
    return EncodeRequirement(format: .jpeg, mode: .lossy, quality: 50)
  }

  public static var lossy5Jpeg: EncodeRequirement {
    return EncodeRequirement(format: .jpeg, mode: .lossy, quality: 5)
  }

  public static var losslessJpeg: EncodeRequirement {
    return EncodeRequirement(format: .jpeg, mode: .lossless)
  }
}
