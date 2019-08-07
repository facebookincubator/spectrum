// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/image/metadata/Entry.h>

#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace image {
namespace metadata {
namespace test {

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error Tests are only expected to be ran on a little-endian machine
#endif

namespace {
static void testSerialisation(
    const Entry& entry,
    const std::vector<std::uint8_t>& expectedData,
    const std::vector<std::uint8_t>& expectedValuesData = {}) {
  const std::uint32_t sizeOffset = 42;
  auto data = std::vector<std::uint8_t>{};
  auto valuesData = std::vector<std::uint8_t>{};
  entry.insertIntoDataAndValuesData(data, valuesData, sizeOffset);

  ASSERT_EQ(expectedData, data);
  ASSERT_EQ(expectedValuesData, valuesData);
}

Entry::TagMap parseEntriesFromLayouts(
    const std::vector<Entry::MemoryLayout>& layouts,
    const std::uint16_t count) {
  auto data = utils::makeDataFromValue(count);
  const auto layoutsData = utils::makeDataFromValue(layouts);
  data.insert(data.end(), layoutsData.begin(), layoutsData.end());

  const auto context =
      ReadContext{data.data(), data.size(), data.data() + 1, true};

  auto entries = Entry::TagMap{};
  Entry::parseFromAddressIntoTagMap(
      context, reinterpret_cast<const std::uint8_t*>(data.data()), entries);
  return entries;
}
} // namespace

//
// Byte
//

TEST(image_metadata_Entry, whenByteEntry_thenExtractedValueIsCorrect) {
  const auto value = std::uint8_t{0x01};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::BYTE, 1, value};

  ASSERT_EQ(value, entry.valueAsByte());
}

TEST(image_metadata_Entry, whenByteVectorEntry_thenExtractedValueIsCorrect) {
  const auto value = std::vector<std::uint8_t>{0x01, 0x02, 0x03, 0x04, 0xFF};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::BYTE, 5, value};

  ASSERT_EQ(value, entry.valueAsByteVector());
}

//
// ASCII
//

TEST(
    image_metadata_Entry,
    whenExtraNullTerminateAsciiEntry_thenExtractedStringIsCorrect) {
  const auto entry =
      Entry{Entry::IMAGE_DESCRIPTION,
            Entry::ASCII,
            6,
            std::vector<std::uint8_t>{'t', 'e', 's', 't', '\0', '\0'}};
  ASSERT_EQ("test", entry.valueAsAsciiString());
}

TEST(
    image_metadata_Entry,
    whenNonNullTerminatedAsciiEntry_thenExtractedStringIsCorrect) {
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION,
                           Entry::ASCII,
                           4,
                           std::vector<std::uint8_t>{'t', 'e', 's', 't'}};
  ASSERT_EQ("test", entry.valueAsAsciiString());
}

TEST(image_metadata_Entry, whenEmptyAsciiEntry_thenExtractedStringIsCorrect) {
  const auto entry = Entry{
      Entry::IMAGE_DESCRIPTION, Entry::ASCII, 0, std::vector<std::uint8_t>{}};
  ASSERT_EQ("", entry.valueAsAsciiString());
}

TEST(image_metadata_Entry, whenEntryValueIsOdd_thenOneByteOfPadding) {
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION,
                           Entry::ASCII,
                           5,
                           std::vector<std::uint8_t>{'a', 'b', 'c', 'd', '\0'}};
  auto data = std::vector<std::uint8_t>{};
  auto valuesData = std::vector<std::uint8_t>{};

  entry.insertIntoDataAndValuesData(data, valuesData, 0);

  // 5 Characters + 1 byte of padding.
  ASSERT_EQ(6, valuesData.size());
}

TEST(
    image_metadata_Entry,
    whenEmptyAsciiWithNullCharacterEntry_thenExtractedStringIsCorrect) {
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION,
                           Entry::ASCII,
                           1,
                           std::vector<std::uint8_t>{'\0'}};
  ASSERT_EQ("", entry.valueAsAsciiString());
}

//
// Short
//

TEST(image_metadata_Entry, whenShortEntry_thenExtractedValueIsCorrect) {
  const auto value = std::uint16_t{0xFBBF};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::SHORT, 1, value};

  ASSERT_EQ(value, entry.valueAsShort());
  ASSERT_EQ(value, entry.valueAsShortOrLong());
}

TEST(image_metadata_Entry, whenShortVectorEntry_thenExtractedValueIsCorrect) {
  const auto value = std::vector<std::uint16_t>{0xFBBF, 0xAECB};
  const auto longValue = std::vector<std::uint32_t>{value.begin(), value.end()};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::SHORT, 2, value};

  ASSERT_EQ(value, entry.valueAsShortVector());
  ASSERT_EQ(longValue, entry.valueAsShortOrLongVector());
}

//
// Long
//

TEST(image_metadata_Entry, whenLongEntry_thenExtractedValueIsCorrect) {
  const auto value = std::uint32_t{0xFBACEA32};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::LONG, 1, value};

  ASSERT_EQ(value, entry.valueAsLong());
  ASSERT_EQ(value, entry.valueAsShortOrLong());
}

TEST(image_metadata_Entry, whenLongVectorEntry_thenExtractedValueIsCorrect) {
  const auto value = std::vector<std::uint32_t>{0xFBACEA32, 0xFBAADA32};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::LONG, 2, value};

  ASSERT_EQ(value, entry.valueAsLongVector());
  ASSERT_EQ(value, entry.valueAsShortOrLongVector());
}

//
// Rational
//

TEST(image_metadata_Entry, whenRationalEntry_thenExtractedValueIsCorrect) {
  const auto value = Rational{0xFBACEA32, 0xAEBDFE13};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::RATIONAL, 1, value};

  ASSERT_EQ(value, entry.valueAsRational());
}

TEST(
    image_metadata_Entry,
    whenRationalVectorEntry_thenExtractedValueIsCorrect) {
  const auto value = std::vector<Rational>{
      {0xFBACEA32, 0xAEBDFE13},
      {0xFBACEA42, 0xAEBDFE87},
  };
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::RATIONAL, 2, value};

  ASSERT_EQ(value, entry.valueAsRationalVector());
}

//
// Undefined
//

TEST(image_metadata_Entry, whenUndefinedEntry_thenExtractedValueIsCorrect) {
  const auto value = std::uint8_t{0x01};
  const auto entry =
      Entry{Entry::IMAGE_DESCRIPTION, Entry::UNDEFINED, 1, value};

  ASSERT_EQ(value, entry.valueAsUndefined());
}

TEST(
    image_metadata_Entry,
    whenUndefinedVectorEntry_thenExtractedValueIsCorrect) {
  const auto value = std::vector<std::uint8_t>{0x01, 0x02, 0x03, 0x04, 0xFF};
  const auto entry =
      Entry{Entry::IMAGE_DESCRIPTION, Entry::UNDEFINED, 5, value};

  ASSERT_EQ(value, entry.valueAsUndefinedVector());
}

//
// Error Validation
//

TEST(image_metadata_Entry, whenExtractingValueWithWrongType_thenThrows) {
  const auto value = std::uint8_t{0x01};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::BYTE, 1, value};

  ASSERT_THROW(entry.valueAsShort(), SpectrumException);
}

TEST(image_metadata_Entry, whenExtractingValueVectorWithWrongType_thenThrows) {
  const auto value = std::vector<std::uint8_t>{0x01, 0x02};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::BYTE, 2, value};

  ASSERT_THROW(entry.valueAsShortVector(), SpectrumException);
}

TEST(image_metadata_Entry, whenExtractingValueVectorWithWrongCount_thenThrows) {
  const auto value = std::vector<std::uint8_t>{0x01, 0x02};
  const auto entry = Entry{Entry::IMAGE_DESCRIPTION, Entry::BYTE, 2, value};

  ASSERT_THROW(entry.valueAsByte(), SpectrumException);
}

//
// Parse Validation Errors
//

TEST(
    image_metadata_Entry,
    whenParsingButDataLengthIsNotBigEnoughForCount_thenThrows) {
  // create array to ensure order of pointers
  const std::array<std::uint8_t, 2> data = {0, 0};
  const auto context = ReadContext{&data[0], std::size_t{2}, &data[1], true};
  auto entries = Entry::TagMap{};

  ASSERT_THROW(
      Entry::parseFromAddressIntoTagMap(context, context.dataEnd - 1, entries),
      SpectrumException);
}

TEST(
    image_metadata_Entry,
    whenParsingButDataLengthIsNotBigEnoughForAllEntries_thenThrows) {
  // create array to ensure order of pointers
  std::uint16_t countEntries = 42;
  const std::array<std::uint16_t, 2> data = {countEntries, 0};
  const auto countAddress = reinterpret_cast<const std::uint8_t*>(&data[0]);
  const auto context = ReadContext{
      countAddress,
      sizeof(std::uint16_t) + (countEntries * sizeof(Entry::MemoryLayout)) - 1,
      reinterpret_cast<const std::uint8_t*>(&data[1]),
      true};
  auto entries = Entry::TagMap{};

  ASSERT_THROW(
      Entry::parseFromAddressIntoTagMap(context, countAddress, entries),
      SpectrumException);
}

//
// Deserialise
//

TEST(image_metadata_Entry, whenValidMemoryLayouts_thenParsedEntriesMatch) {
  const auto entries = parseEntriesFromLayouts(
      {
          Entry::MemoryLayout{Entry::IMAGE_DESCRIPTION,
                              Entry::SHORT,
                              2,
                              std::uint32_t{0xDEADBEEF}},
          Entry::MemoryLayout{
              Entry::EXIF_VERSION, Entry::SHORT, 2, std::uint32_t{0xABCD1234}},
      },
      2);

  const auto expectedFirstEntry =
      Entry{Entry::IMAGE_DESCRIPTION,
            Entry::SHORT,
            2,
            std::vector<std::uint16_t>{0xBEEF, 0xDEAD}};
  const auto expectedSecondEntry =
      Entry{Entry::EXIF_VERSION,
            Entry::SHORT,
            2,
            std::vector<std::uint16_t>{0x1234, 0xABCD}};

  ASSERT_EQ(2, entries.size());
  ASSERT_EQ(expectedFirstEntry, entries.find(Entry::IMAGE_DESCRIPTION)->second);
  ASSERT_EQ(expectedSecondEntry, entries.find(Entry::EXIF_VERSION)->second);
}

TEST(
    image_metadata_Entry,
    whenInvalidEntry_thenSkipItAndParseFollowingEntries) {
  const auto entries = parseEntriesFromLayouts(
      {
          Entry::MemoryLayout{
              Entry::IMAGE_DESCRIPTION,
              0,
              1,
              std::uint16_t{0}}, // Invalid type, entry will be skipped.
          Entry::MemoryLayout{
              Entry::EXIF_VERSION, Entry::SHORT, 2, std::uint32_t{0xABCD1234}},
      },
      2);

  const auto expectedEntry = Entry{Entry::EXIF_VERSION,
                                   Entry::SHORT,
                                   2,
                                   std::vector<std::uint16_t>{0x1234, 0xABCD}};
  ASSERT_EQ(1, entries.size());
  ASSERT_EQ(expectedEntry, entries.find(Entry::EXIF_VERSION)->second);
}

//
// Serialise
//

TEST(
    image_metadata_Entry,
    whenSerialisingValueSmallerThanValueOffset_thenFieldsAreCorrect) {
  testSerialisation(
      Entry{Entry::ORIENTATION, Entry::SHORT, std::uint16_t{0xBADF}},
      {
          0x12,
          0x01, // Tag
          0x03,
          0x00, // Type
          0x01,
          0x00,
          0x00,
          0x00, // Count
          0xDF,
          0xBA,
          0x00,
          0x00 // ValueOffset
      });
} // namespace metadata

TEST(
    image_metadata_Entry,
    whenSerialisingAsBigAsValueOffset_thenFieldsAreCorrect) {
  testSerialisation(
      Entry{Entry::ORIENTATION, Entry::LONG, std::uint32_t{0x8BADF00D}},
      {
          0x12,
          0x01, // Tag
          0x04,
          0x00, // Type
          0x01,
          0x00,
          0x00,
          0x00, // Count
          0x0D,
          0xF0,
          0xAD,
          0x8B // ValueOffset
      });
} // namespace base

TEST(
    image_metadata_Entry,
    whenSerialisingBiggerThanValueOffsetSize_thenFieldsAreCorrect) {
  testSerialisation(
      Entry{Entry::ORIENTATION,
            Entry::LONG,
            std::vector<std::uint32_t>{0x8BADF00D, 0xDEADBEEF, 0xABCD1234}},
      {
          0x12,
          0x01, // Tag
          0x04,
          0x00, // Type
          0x03,
          0x00,
          0x00,
          0x00, // Count
          0x2A,
          0x00,
          0x00,
          0x00 // ValueOffset
      },
      {
          0x0D,
          0xF0,
          0xAD,
          0x8B, // Val 1
          0xEF,
          0xBE,
          0xAD,
          0xDE, // Val 2
          0x34,
          0x12,
          0xCD,
          0xAB, // Val 3
      });
}

} // namespace test
} // namespace metadata
} // namespace image
} // namespace spectrum
} // namespace facebook
