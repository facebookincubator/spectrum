// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "Numeric.h"

#include <cmath>

namespace facebook {
namespace spectrum {
namespace core {
namespace numeric {

double rounded(const double value, const RoundingMode roundingMode) {
  switch (roundingMode) {
    case RoundingMode::Up:
      return ceil(value);
    case RoundingMode::Nearest:
      return round(value);
    case RoundingMode::Down:
      return floor(value);
  }
}

} // namespace numeric
} // namespace core
} // namespace spectrum
} // namespace facebook
