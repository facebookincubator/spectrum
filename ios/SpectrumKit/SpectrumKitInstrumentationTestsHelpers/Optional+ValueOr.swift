// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

import Foundation

extension Optional {
  func valueOrThrow(_ error: Error) throws -> Wrapped {
    switch self {
    case let .some(value):
      return value
    case .none:
      throw error
    }
  }
}
