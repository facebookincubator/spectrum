// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <cmath>
#include <cstddef>
#include <iterator>
#include <vector>

#include <spectrum/core/DataRange.h>

#include <folly/Range.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace error {
extern const folly::StringPiece InvalidProfileHeader;
extern const folly::StringPiece ProfileTooLargeForMarker;
extern const folly::StringPiece UnexpectedProfileChunkIndex;
extern const folly::StringPiece UnexpectedProfileChunkTotal;
} // namespace error

class ICCProfile {
  std::vector<std::uint8_t> _data;

 public:
  explicit ICCProfile() = default;
  explicit ICCProfile(const std::vector<std::uint8_t>& data);
  explicit ICCProfile(const std::vector<core::DataRange>& chunks);

  std::vector<std::vector<std::uint8_t>> makeEncodedData() const;

  bool empty() const;
  std::size_t size() const;
  const std::vector<std::uint8_t>& data() const;

  bool operator==(const ICCProfile& other) const;
};

} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
