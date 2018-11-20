// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

extension UIImage {
  func similarityFactor(with image: UIImage) -> CGFloat {
    return FSPComputeSSIMFactorBetween(self.transformToUpOrientation(), image.transformToUpOrientation())
  }
}
