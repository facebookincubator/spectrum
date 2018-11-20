// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <cstddef>

namespace facebook {
namespace spectrum {
namespace core {

/**
 * The default buffer size to use for operations that copy from one location
 * to another.
 */
static constexpr const std::size_t DefaultBufferSize = 4096;

} // namespace core
} // namespace spectrum
} // namespace facebook
