// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/metadata/ICCProfile.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace test {
namespace {

static constexpr std::size_t MARKER_MAX_SIZE = 1024 * 64; // 64k
static constexpr const char* MARKER_HEADER_TAG = "ICC_PROFILE";
static constexpr std::size_t MARKER_HEADER_TAG_SIZE = 12; /* inc. \0 */
static constexpr std::size_t MARKER_HEADER_SIZE =
    MARKER_HEADER_TAG_SIZE + 2; /* current marker + total marker */
static constexpr std::size_t MARKER_PAYLOAD_SIZE =
    MARKER_MAX_SIZE - MARKER_HEADER_SIZE;

std::vector<std::uint8_t> makeEncodedDataForData(
    const std::vector<std::uint8_t>& data,
    const std::uint8_t currentMarker = 1,
    const std::uint8_t totalMarkers = 1) {
  std::vector<std::uint8_t> packedData(
      MARKER_HEADER_TAG, MARKER_HEADER_TAG + MARKER_HEADER_TAG_SIZE);

  packedData.push_back(currentMarker);
  packedData.push_back(totalMarkers);

  packedData.insert(packedData.cend(), data.cbegin(), data.cend());
  return {packedData};
}

ICCProfile makeProfileForData(
    const std::vector<std::vector<std::uint8_t>>& vectorData) {
  std::vector<core::DataRange> dataRanges;

  for (const auto& data : vectorData) {
    dataRanges.push_back(core::DataRange{data.data(), data.size()});
  }

  return ICCProfile{dataRanges};
}

} // namespace

//
// Serialisation
//

TEST(
    image_metadata_ICCProfile,
    whenSerialisingAProfileWithNoData_thenCreatesEmptyArray) {
  const auto profileData = ICCProfile{}.makeEncodedData();

  ASSERT_TRUE(profileData.empty());
}

TEST(
    image_metadata_ICCProfile,
    whenSerialisingAProfileWithOneChunk_thenSingleChunkReturned) {
  const std::vector<std::uint8_t> data{'0', '1', '2'};
  const auto profileData = ICCProfile{data}.makeEncodedData();
  const std::vector<std::vector<std::uint8_t>> expectedProfileData = {
      makeEncodedDataForData(data),
  };

  ASSERT_EQ(expectedProfileData, profileData);
}

TEST(
    image_metadata_ICCProfile,
    whenSerialisingAProfileWithTwoChunks_thenTwoChunksReturned) {
  // Data fits withink 64k, but the header will make it outgrow the limit -
  // hence the expected 2 chunks.
  const auto profileData =
      ICCProfile{std::vector<std::uint8_t>(MARKER_MAX_SIZE, 42)}
          .makeEncodedData();
  const std::vector<std::vector<std::uint8_t>> expectedProfileData = {
      makeEncodedDataForData(
          std::vector<std::uint8_t>(MARKER_PAYLOAD_SIZE, 42), 1, 2),
      makeEncodedDataForData(
          std::vector<std::uint8_t>(MARKER_HEADER_SIZE, 42), 2, 2),
  };

  ASSERT_EQ(expectedProfileData, profileData);
}

//
// Deserialisation
//

TEST(
    image_metadata_ICCProfile,
    whenDeserialisingAProfileWithNotEnoughChunks_thenSkipped) {
  const auto profileData = makeEncodedDataForData(
      {'a', 'b', 'c'}, 1, 2); // Invalid current index, should be 1.
  const auto profile =
      ICCProfile({core::DataRange{profileData.data(), profileData.size()}});

  ASSERT_TRUE(profile.empty());
}

TEST(
    image_metadata_ICCProfile,
    whenDeserialisingAProfileWithCurrentIndexGreaterThanTotalCount_thenSkipped) {
  const auto profileData = makeEncodedDataForData(
      {'a', 'b', 'c'}, 1, 0); // Invalid current index, should be 1.
  const auto profile =
      ICCProfile({core::DataRange{profileData.data(), profileData.size()}});

  ASSERT_TRUE(profile.empty());
}

TEST(
    image_metadata_ICCProfile,
    whenDeserialisingAProfileWithInvalidCurrentIndexes_thenSkipped) {
  const auto profileData = makeEncodedDataForData(
      {'a', 'b', 'c'}, 0, 1); // Invalid current index, should be 1.
  const auto profile = makeProfileForData({profileData});

  ASSERT_TRUE(profile.empty());
}

TEST(
    image_metadata_ICCProfile,
    whenDeserialisingAProfileWithSingleChunk_thenCorrect) {
  const std::vector<std::uint8_t> data{'1', '2', '3'};
  const auto profileData = makeEncodedDataForData(data);
  const auto profile = makeProfileForData({profileData});

  ASSERT_EQ(data, profile.data());
}

TEST(
    image_metadata_ICCProfile,
    whenDeserialisingAProfileWithTwoChunks_thenCorrect) {
  const auto profileData1 = makeEncodedDataForData(
      std::vector<std::uint8_t>(MARKER_PAYLOAD_SIZE, 42), 1, 2);
  const auto profileData2 =
      makeEncodedDataForData(std::vector<std::uint8_t>(5, 42), 2, 2);
  const auto profile = makeProfileForData({profileData1, profileData2});
  const std::vector<std::uint8_t> expectedProfileData(
      MARKER_PAYLOAD_SIZE + 5, 42);

  ASSERT_EQ(expectedProfileData, profile.data());
}

TEST(
    image_metadata_ICCProfile,
    whenDeserialisingAProfileWithTwoNonContiguousChunks_thenSkipped) {
  const auto profileData1 = makeEncodedDataForData(
      std::vector<std::uint8_t>(MARKER_PAYLOAD_SIZE, 42), 1, 2);
  const auto profileData2 =
      makeEncodedDataForData(std::vector<std::uint8_t>(5, 42), 1, 2);
  const auto profile = makeProfileForData({profileData1, profileData2});

  ASSERT_TRUE(profile.data().empty());
}
} // namespace test
} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
