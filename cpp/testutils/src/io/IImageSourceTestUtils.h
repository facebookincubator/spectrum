// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <spectrum/io/IImageSource.h>
#include <spectrum/io/VectorImageSource.h>

namespace facebook {
namespace spectrum {
namespace io {
namespace testutils {

/**
 * Performs and asserts the following:
 *  - read up to lenToRead bytes from imageSource into a temporary buffer
 *  - ASSERT that read number of bytes matches length of expectedString
 *  - ASSERT that those bytes read matches expectedString content
 */
void assertRead(
    const std::string& expectedString,
    const std::size_t lenToRead,
    IImageSource& imageSource);

VectorEncodedImageSource makeVectorImageSource(const std::string& content);

VectorBitmapImageSource makeVectorBitmapImageSource(
    const std::string& content,
    const image::Specification& imageSpecification);

} // namespace testutils
} // namespace io
} // namespace spectrum
} // namespace facebook
