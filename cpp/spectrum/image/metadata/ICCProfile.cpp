// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include "ICCProfile.h"

#include <spectrum/core/SpectrumEnforce.h>

#include <cstring> // strncmp
#include <map>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace error {
const folly::StringPiece InvalidProfileHeader{
    "metadata_invalid_profile_header"};
const folly::StringPiece ProfileTooLargeForMarker{
    "metadata_profile_too_large_for_marker"};
const folly::StringPiece UnexpectedProfileChunkIndex{
    "metadata_profile_unexpected_chunk_index"};
const folly::StringPiece UnexpectedProfileChunkTotal{
    "metadata_profile_unexpected_chunk_total"};
} // namespace error

static constexpr std::size_t MARKER_MAX_SIZE = 1024 * 64; // 64k
static constexpr const char* MARKER_HEADER_TAG = "ICC_PROFILE";
static constexpr std::size_t MARKER_HEADER_TAG_SIZE = 12; /* inc. \0 */
static constexpr std::size_t MARKER_HEADER_SIZE =
    MARKER_HEADER_TAG_SIZE + 2; /* current marker + total marker */
static constexpr std::size_t MARKER_PAYLOAD_SIZE =
    MARKER_MAX_SIZE - MARKER_HEADER_SIZE;

namespace {
bool ensureIsProfileData(
    const core::DataRange& dataRange,
    const std::size_t totalCount) {
  return dataRange.length <= MARKER_MAX_SIZE &&
      dataRange.length > MARKER_HEADER_SIZE &&
      std::strncmp(
          reinterpret_cast<const char*>(dataRange.begin),
          MARKER_HEADER_TAG,
          MARKER_HEADER_TAG_SIZE) == 0;
}

std::size_t indexOfChunk(
    const core::DataRange& dataRange,
    std::uint8_t& currentTotalCount) {
  SPECTRUM_ERROR_IF_NOT(
      ensureIsProfileData(dataRange, currentTotalCount),
      error::InvalidProfileHeader);

  const std::uint8_t chunkIndex = *(dataRange.begin + MARKER_HEADER_TAG_SIZE);
  const std::uint8_t chunkTotalCount =
      *(dataRange.begin + MARKER_HEADER_TAG_SIZE + 1);

  if (currentTotalCount != 0) {
    SPECTRUM_ERROR_IF(
        currentTotalCount != chunkTotalCount,
        error::UnexpectedProfileChunkIndex);
  }

  // 1 <= chunkIndex <= chunkTotalCount
  SPECTRUM_ERROR_IF_NOT(
      chunkIndex > 0 && chunkIndex <= chunkTotalCount,
      error::UnexpectedProfileChunkIndex);

  // Update the currentTotalCount with value for the first found byte.
  if (currentTotalCount == 0) {
    currentTotalCount = chunkTotalCount;
  }

  return chunkIndex;
}
} // namespace

ICCProfile::ICCProfile(const std::vector<std::uint8_t>& data) : _data(data) {
  SPECTRUM_ERROR_IF(
      _data.size() >= 255 * MARKER_PAYLOAD_SIZE,
      error::ProfileTooLargeForMarker);
}

ICCProfile::ICCProfile(const std::vector<core::DataRange>& chunks) {
  std::uint8_t currentTotalCount = 0;
  std::map<std::size_t, std::size_t> indexesByChunkIndexes;

  for (std::size_t i = 0; i < chunks.size(); ++i) {
    try {
      indexesByChunkIndexes[indexOfChunk(chunks[i], currentTotalCount)] = i;
    } catch (const SpectrumException& e) {
      // We've got an error. Its not a profile, or an invalid one. Skip it.
    }
  }

  if (currentTotalCount == indexesByChunkIndexes.size()) {
    for (std::size_t i = 1; i <= currentTotalCount; ++i) {
      // Ensure's we've got consecutive indexes. i.e not 3 items with ids 1, 2,
      // & 5.
      const auto chunk = chunks[indexesByChunkIndexes[i]];
      if (chunk.length > MARKER_HEADER_SIZE) {
        _data.insert(
            _data.cend(), chunk.begin + MARKER_HEADER_SIZE, chunk.end());
      }
    }
  }
}

std::vector<std::vector<std::uint8_t>> ICCProfile::makeEncodedData() const {
  const auto totalChunks =
      (_data.size() + MARKER_PAYLOAD_SIZE - 1) / MARKER_PAYLOAD_SIZE;
  std::vector<std::vector<std::uint8_t>> dataChunks;

  for (auto iterator = _data.cbegin(); iterator < _data.cend();
       iterator += MARKER_PAYLOAD_SIZE) {
    const std::size_t dataLeft = std::distance(iterator, _data.cend());
    const auto dataSize = std::min(MARKER_PAYLOAD_SIZE, dataLeft);
    std::vector<std::uint8_t> data(
        MARKER_HEADER_TAG, MARKER_HEADER_TAG + MARKER_HEADER_TAG_SIZE);

    // Header element: marker id
    data.push_back(static_cast<std::uint8_t>(dataChunks.size() + 1));
    // Header element: total marker count
    data.push_back(static_cast<std::uint8_t>(totalChunks));

    data.insert(data.end(), iterator, iterator + dataSize);
    dataChunks.emplace_back(std::move(data));
  }

  return dataChunks;
}

std::size_t ICCProfile::size() const {
  return _data.size();
}

bool ICCProfile::empty() const {
  return _data.empty();
}

const std::vector<std::uint8_t>& ICCProfile::data() const {
  return _data;
}

bool ICCProfile::operator==(const ICCProfile& other) const {
  return _data == other._data;
}

} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
