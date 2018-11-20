// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

namespace facebook {
namespace spectrum {
namespace image {
namespace testutils {

static constexpr auto DefaultImageSize = image::Size{16, 32};
}
} // namespace image
} // namespace spectrum
} // namespace facebook

#include <spectrum/testutils/image/PixelTestUtils.h>
#include <spectrum/testutils/image/ScanlineTestUtils.h>
#include <spectrum/testutils/image/SpecificationTestUtils.h>
