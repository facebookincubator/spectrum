// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <cstdint>
#include <vector>

namespace facebook {
namespace spectrum {
namespace image {
namespace pixel {
namespace testutils {

using Initializer = std::vector<std::uint8_t>;
using InitializerVector = std::vector<Initializer>;

} // namespace testutils
} // namespace pixel
} // namespace image
} // namespace spectrum
} // namespace facebook
