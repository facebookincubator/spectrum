// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <cstddef>

namespace facebook {
namespace spectrum {
namespace plugins {
namespace jpeg {

/**
 * The JPEG marker representing the APP1 section containing EXIF.
 */
static constexpr std::uint8_t JPEG_APP1 = 0xE1;
static constexpr std::uint8_t JPEG_APP2 = 0xE2;

static constexpr std::uint32_t maximumSizeDimension = 65535;

} // namespace jpeg
} // namespace plugins
} // namespace spectrum
} // namespace facebook
