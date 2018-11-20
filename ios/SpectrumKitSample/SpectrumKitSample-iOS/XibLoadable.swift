// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import UIKit

protocol XibLoadable {
  static var xibName: String { get }
}

extension XibLoadable {
  static func loadFromXib() -> Self {
    let views = Bundle.main.loadNibNamed(self.xibName, owner: nil, options: nil)

    guard let view = views?.first as? Self else {
      fatalError("Failed to load view from xib nammed: \(self.xibName) as \(String(describing: self))")
    }

    return view
  }
}

extension XibLoadable where Self: UIView {
  static var xibName: String {
    return String(describing: self)
  }
}

extension UIView: XibLoadable {}
