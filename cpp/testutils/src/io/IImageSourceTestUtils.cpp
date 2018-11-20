// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "IImageSourceTestUtils.h"

#include <spectrum/testutils/image/SpecificationTestUtils.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace io {
namespace testutils {

void assertRead(
    const std::string& expectedString,
    const std::size_t lenToRead,
    IImageSource& imageSource) {
  const auto expectedLen = expectedString.size();
  auto buffer = std::vector<char>(lenToRead, '\0');
  const auto actualBytesRead = imageSource.read(buffer.data(), buffer.size());

  // assert number of characters returned as read match the expected string
  // length
  ASSERT_EQ(expectedLen, actualBytesRead);

  // create string from buffer and compare to expected string
  ASSERT_EQ(expectedString, std::string(buffer.data(), actualBytesRead));
}

VectorEncodedImageSource makeVectorImageSource(const std::string& content) {
  auto v = std::vector<char>{content.begin(), content.end()};
  return VectorEncodedImageSource(std::move(v));
}

VectorBitmapImageSource makeVectorBitmapImageSource(
    const std::string& content,
    const image::Specification& imageSpecification) {
  auto v = std::vector<char>{content.begin(), content.end()};
  return VectorBitmapImageSource(std::move(v), imageSpecification);
}

} // namespace testutils
} // namespace io
} // namespace spectrum
} // namespace facebook
