// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/image/Specification.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace testutils {

inline image::Specification makeDummyImageSpecification(
    const image::Format& format = image::formats::Bitmap,
    const image::pixel::Specification& pixelSpecification =
        image::pixel::specifications::RGBA) {
  return image::Specification{
      .size = image::Size{.width = 1, .height = 1},
      .format = format,
      .pixelSpecification = pixelSpecification,
      .chromaSamplingMode = image::ChromaSamplingMode::S444,
      .orientation = image::Orientation::Up,
      .metadata = {},
  };
}

} // namespace testutils
} // namespace image
} // namespace spectrum
} // namespace facebook
