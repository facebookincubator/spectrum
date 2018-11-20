// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/metadata/Entries.h>
#include <spectrum/image/metadata/Utils.h>

#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <vector>

#include <folly/Optional.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace test {

namespace { /* anonymous */
std::vector<std::uint8_t> makeTestLayout(
    const folly::Optional<Entries::MemoryLayout>& layout = folly::none,
    const std::uint16_t countOfEntries = 0) {
  auto data =
      utils::makeDataFromValue(layout.value_or(Entries::MemoryLayout{}));
  const auto countOfEntriesData = utils::makeDataFromValue(countOfEntries);
  data.insert(data.end(), countOfEntriesData.begin(), countOfEntriesData.end());
  return data;
}

Entry::TagMap makeTagMapFromEntriesVector(const std::vector<Entry>& entries) {
  auto entriesMap = Entry::TagMap{};

  for (auto& entry : entries) {
    entriesMap.insert(std::make_pair(entry.getTag(), entry));
  }

  return entriesMap;
}

Entries makeEntries(
    const std::vector<Entry>& tiffEntries,
    const std::vector<Entry>& exifEntries = {},
    const std::vector<Entry>& gpsEntries = {}) {
  return {makeTagMapFromEntriesVector(tiffEntries),
          makeTagMapFromEntriesVector(exifEntries),
          makeTagMapFromEntriesVector(gpsEntries)};
}

using EntriesDataPair = std::pair<Entries, std::vector<std::uint8_t>>;

EntriesDataPair makeEntriesData(
    const std::vector<Entry>& tiffEntries,
    const std::vector<Entry>& exifEntries = {},
    const std::vector<Entry>& gpsEntries = {}) {
  const auto entries = makeEntries(tiffEntries, exifEntries, gpsEntries);
  return std::make_pair(entries, entries.makeData());
}
} // namespace

TEST(
    image_metadata_Entries,
    whenDataLengthIsNotBigEnoughForMemoryLayout_thenThrows) {
  const auto layout = Entries::MemoryLayout{};
  ASSERT_THROW(
      layout.ensureExpectedLayout(sizeof(Entries::MemoryLayout) - 1),
      SpectrumException);
}

TEST(image_metadata_Entries, whenExifHeaderDoesNotContainExif_thenThrows) {
  const auto layout = Entries::MemoryLayout{"NotExif"};
  ASSERT_THROW(
      layout.ensureExpectedLayout(sizeof(Entries::MemoryLayout)),
      SpectrumException);
}

TEST(
    image_metadata_Entries,
    whenExifHeaderDoesNotContainValidEndianCode_thenThrows) {
  const auto layout =
      Entries::MemoryLayout{Entries::MemoryLayout::HEADER_VALUE,
                            Entries::MemoryLayout::ENDIAN_CODE_CURRENT - 1};
  ASSERT_THROW(
      layout.ensureExpectedLayout(sizeof(Entries::MemoryLayout)),
      SpectrumException);
}

TEST(
    image_metadata_Entries,
    whenExifHeaderDoesNotContainValidFixedValue_thenThrows) {
  const auto layout =
      Entries::MemoryLayout{Entries::MemoryLayout::HEADER_VALUE,
                            Entries::MemoryLayout::ENDIAN_CODE_CURRENT,
                            0};
  ASSERT_THROW(
      layout.ensureExpectedLayout(sizeof(Entries::MemoryLayout)),
      SpectrumException);
}

TEST(image_metadata_Entries, whenMerging_thenAllEntriesGetMerged) {
  auto entries = Entries{};
  const auto other = makeEntries(
      {Entry{Entry::Tag::MAKE,
             Entry::Type::ASCII,
             std::vector<std::uint8_t>{'B', '\0'}}},
      {Entry{Entry::Tag::MODEL,
             Entry::Type::ASCII,
             std::vector<std::uint8_t>{'C', '\0'}}},
      {Entry{Entry::Tag::COPYRIGHT,
             Entry::Type::ASCII,
             std::vector<std::uint8_t>{'D', '\0'}}});

  entries.merge(other);
  ASSERT_EQ(entries, other);
}

TEST(
    image_metadata_Entries,
    whenMerging_thenEntriesFromThisAreOverridenByOthers) {
  auto entries = makeEntries({Entry{Entry::Tag::MAKE,
                                    Entry::Type::ASCII,
                                    std::vector<std::uint8_t>{'A', '\0'}}});
  const auto other = makeEntries({Entry{Entry::Tag::MAKE,
                                        Entry::Type::ASCII,
                                        std::vector<std::uint8_t>{'B', '\0'}}});

  entries.merge(other);

  ASSERT_EQ("B", entries.tiff().at(Entry::Tag::MAKE).valueAsAsciiString());
}

TEST(image_metadata_Entries, whenFirstIfdOffsetPointsInsideTheMemoryLayout) {
  const auto layout =
      Entries::MemoryLayout{Entries::MemoryLayout::HEADER_VALUE,
                            Entries::MemoryLayout::ENDIAN_CODE_CURRENT,
                            Entries::MemoryLayout::FIXED_VALUE,
                            Entries::MemoryLayout::DEFAULT_OFFSET - 1};
  ASSERT_THROW(
      layout.ensureExpectedLayout(sizeof(Entries::MemoryLayout)),
      SpectrumException);
}

TEST(image_metadata_Entries, whenParsingLayoutAlsoValidatesHeader_thenThrows) {
  const auto layout = Entries::MemoryLayout{"NotExif"};
  const auto data = makeTestLayout(layout);

  ASSERT_THROW(Entries(data.data(), data.size()), SpectrumException);
}

TEST(
    image_metadata_Entries,
    whenParsingButFirstAttributeCountIsMissing_thenThrows) {
  const auto data = utils::makeDataFromValue(Entries::MemoryLayout{});

  ASSERT_THROW(Entries(data.data(), data.size()), SpectrumException);
}

TEST(image_metadata_Entries, whenParsingButNoAttributes_thenSizeIsZero) {
  const auto data = makeTestLayout();
  const auto entries = Entries{data.data(), data.size()};

  ASSERT_EQ(0, entries.entriesSize());
}

TEST(
    image_metadata_Entries,
    whenTiffContainsOffsetButTheIfdHasNoData_thenTheyAreNotWritten) {
  const auto entries = makeEntriesData({
      Entry{Entry::IMAGE_DESCRIPTION,
            Entry::ASCII,
            std::vector<std::uint8_t>{'W', 'o', 'w', '\0'}},
      Entry{Entry::EXIF_IFD_POINTER, Entry::LONG, std::uint32_t{0x8BADF00D}},
      Entry{
          Entry::GPS_INFO_IFD_POINTER, Entry::LONG, std::uint32_t{0xDEADBEEF}},
  });

  const auto newEntries = Entries{entries.second.data(), entries.second.size()};

  ASSERT_EQ(
      "Wow",
      newEntries.tiff().at(Entry::IMAGE_DESCRIPTION).valueAsAsciiString());
  ASSERT_EQ(
      newEntries.tiff().end(), newEntries.tiff().find(Entry::EXIF_IFD_POINTER));
  ASSERT_EQ(
      newEntries.tiff().end(),
      newEntries.tiff().find(Entry::GPS_INFO_IFD_POINTER));
  ASSERT_EQ(1, newEntries.entriesSize());
}

TEST(image_metadata_Entries, whenExifDataPresent_thenOffsetAndEntriesAdded) {
  const auto entries = makeEntriesData(
      {
          Entry{Entry::IMAGE_DESCRIPTION,
                Entry::ASCII,
                std::vector<std::uint8_t>{'W', 'o', 'w', '\0'}},
      },
      {
          Entry{Entry::EXIF_VERSION, Entry::LONG, std::uint32_t{0x8BADF00D}},
      });

  const auto newEntries = Entries{entries.second.data(), entries.second.size()};

  ASSERT_EQ(
      "Wow",
      newEntries.tiff().at(Entry::IMAGE_DESCRIPTION).valueAsAsciiString());
  ASSERT_EQ(38, newEntries.tiff().at(Entry::EXIF_IFD_POINTER).valueAsLong());
  ASSERT_EQ(2, newEntries.tiff().size());
  ASSERT_EQ(
      0x8BADF00D, newEntries.exif().at(Entry::EXIF_VERSION).valueAsLong());
  ASSERT_EQ(1, newEntries.exif().size());
}

TEST(image_metadata_Entries, whenGpsDataPresent_thenOffsetAndEntriesAdded) {
  const auto entries = makeEntriesData(
      {
          Entry{Entry::IMAGE_DESCRIPTION,
                Entry::ASCII,
                std::vector<std::uint8_t>{'W', 'o', 'w', '\0'}},
      },
      {},
      {
          Entry{Entry::GPS_SPEED, Entry::LONG, std::uint32_t{0x8BADF00D}},
      });

  const auto newEntries = Entries{entries.second.data(), entries.second.size()};

  ASSERT_EQ(
      "Wow",
      newEntries.tiff().at(Entry::IMAGE_DESCRIPTION).valueAsAsciiString());
  ASSERT_EQ(
      38, newEntries.tiff().at(Entry::GPS_INFO_IFD_POINTER).valueAsLong());
  ASSERT_EQ(2, newEntries.tiff().size());
  ASSERT_EQ(0x8BADF00D, newEntries.gps().at(Entry::GPS_SPEED).valueAsLong());
  ASSERT_EQ(1, newEntries.gps().size());
}

TEST(
    image_metadata_Entries,
    whenExifAndGpsIfdReferenceValues_thenItCorrectlySerialisesTheValues) {
  const auto entries = makeEntriesData(
      {
          Entry{Entry::IMAGE_DESCRIPTION,
                Entry::ASCII,
                std::vector<std::uint8_t>{'W', 'o', 'w', '\0'}},
          Entry{Entry::ARTIST,
                Entry::ASCII,
                std::vector<std::uint8_t>{
                    'W', 'o', 'w', ' ', 'c', 'r', 'a', 'z', 'y', '\0'}},
      },
      {
          Entry{Entry::USER_COMMENT, Entry::LONG, std::uint32_t{0x8F00DBAD}},
          Entry{Entry::MAKER_NOTE,
                Entry::ASCII,
                std::vector<std::uint8_t>{
                    'W', 'u', 'w', ' ', 'c', 'r', 'a', 'z', 'y', '\0'}},
      },
      {
          Entry{Entry::GPS_SPEED, Entry::LONG, std::uint32_t{0x8BADF00D}},
          Entry{Entry::GPS_STATUS,
                Entry::ASCII,
                std::vector<std::uint8_t>{
                    'W', 'a', 'w', ' ', 'c', 'r', 'a', 'z', 'y', '\0'}},
      });

  const auto newEntries = Entries{entries.second.data(), entries.second.size()};

  ASSERT_EQ(
      "Wow",
      newEntries.tiff().at(Entry::IMAGE_DESCRIPTION).valueAsAsciiString());
  ASSERT_EQ(
      "Wow crazy", newEntries.tiff().at(Entry::ARTIST).valueAsAsciiString());
  ASSERT_EQ(72, newEntries.tiff().at(Entry::EXIF_IFD_POINTER).valueAsLong());
  ASSERT_EQ(
      112, newEntries.tiff().at(Entry::GPS_INFO_IFD_POINTER).valueAsLong());
  ASSERT_EQ(4, newEntries.tiff().size());

  ASSERT_EQ(
      0x8F00DBAD, newEntries.exif().at(Entry::USER_COMMENT).valueAsLong());
  ASSERT_EQ(
      "Wuw crazy",
      newEntries.exif().at(Entry::MAKER_NOTE).valueAsAsciiString());
  ASSERT_EQ(4, newEntries.tiff().size());

  ASSERT_EQ(0x8BADF00D, newEntries.gps().at(Entry::GPS_SPEED).valueAsLong());
  ASSERT_EQ(
      "Waw crazy", newEntries.gps().at(Entry::GPS_STATUS).valueAsAsciiString());
  ASSERT_EQ(4, newEntries.tiff().size());
}

namespace { /* anonymous */
auto makeEntriesWithOrientation(const short orientationValue) {
  return makeEntries(
      {Entry{Entry::Tag::ORIENTATION, Entry::Type::SHORT, orientationValue}});
}
} // namespace

TEST(EntriesRepresentation, whenImageOrientationValid_thenMatches) {
  ASSERT_EQ(
      image::Orientation::Up, makeEntriesWithOrientation(1).orientation());
  ASSERT_EQ(
      image::Orientation::UpMirrored,
      makeEntriesWithOrientation(2).orientation());
  ASSERT_EQ(
      image::Orientation::Bottom, makeEntriesWithOrientation(3).orientation());
  ASSERT_EQ(
      image::Orientation::BottomMirrored,
      makeEntriesWithOrientation(4).orientation());
  ASSERT_EQ(
      image::Orientation::LeftMirrored,
      makeEntriesWithOrientation(5).orientation());
  ASSERT_EQ(
      image::Orientation::Right, makeEntriesWithOrientation(6).orientation());
  ASSERT_EQ(
      image::Orientation::RightMirrored,
      makeEntriesWithOrientation(7).orientation());
  ASSERT_EQ(
      image::Orientation::Left, makeEntriesWithOrientation(8).orientation());
}

TEST(EntriesRepresentation, whenImageOrientationInvalid_thenNone) {
  ASSERT_FALSE(makeEntriesWithOrientation(0).orientation().hasValue());
  ASSERT_FALSE(makeEntriesWithOrientation(9).orientation().hasValue());
}

TEST(EntriesRepresentation, whenImageOrientationIsMissing_thenNone) {
  ASSERT_FALSE(makeEntries({}).orientation().hasValue());
}

} // namespace test
} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
