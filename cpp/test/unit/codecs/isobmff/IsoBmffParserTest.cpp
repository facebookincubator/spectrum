// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/codecs/isobmff/IsoBmffParser.h>
#include <spectrum/testutils/TestUtils.h>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace codecs {
namespace isobmff {
namespace test {

// NOTE: As we handle const char* with \x00 bytes, we need to specifically call
// the std::string(const char*, size_t) constructor to avoid early stream ends

// NOTE: C++ parser are allowed to parse any \x.... sequence until the first
// invalid character. To connect with non-hex strings we combine two const char*
// literals: str = "\x00""FOO";

//
// parseBoxHeader
//

TEST(codecs_isobmff_Parser, parseBoxHeader_whenEmpty_thenThrowsEarlyStreamEnd) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string("", 0));
  Parser parser(imageSource);
  ASSERT_SPECTRUM_THROW(parser.parseBoxHeader(), error::IsoBmffEarlyStreamEnd);
}

TEST(
    codecs_isobmff_Parser,
    parseBoxHeader_whenOnlySize_thenThrowsEarlyStreamEnd) {
  auto imageSource =
      io::testutils::makeVectorImageSource(std::string("\x00\x00\x00\x04", 4));
  Parser parser(imageSource);
  ASSERT_SPECTRUM_THROW(parser.parseBoxHeader(), error::IsoBmffEarlyStreamEnd);
}

TEST(codecs_isobmff_Parser, parseBoxHeader_whenSizeAndType_thenReturned) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x04"
      "TEST",
      4 + 4));
  Parser parser(imageSource);
  const auto header = parser.parseBoxHeader();
  ASSERT_EQ(4, *header.size);
  spectrum::testutils::assertArrayContent("TEST", header.boxType);
}

TEST(codecs_isobmff_Parser, parseBoxHeader_whenSizeInfinite_thenReturned) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x00"
      "TEST",
      4 + 4));
  Parser parser(imageSource);
  const auto header = parser.parseBoxHeader();
  ASSERT_FALSE(header.size.hasValue());
}

TEST(codecs_isobmff_Parser, parseBoxHeader_whenSizeExtraLong_thenReturned) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x01"
      "TEST"
      "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFE",
      4 + 4 + 8));
  Parser parser(imageSource);
  const auto header = parser.parseBoxHeader();
  ASSERT_EQ(0xFFFFFFFFFFFFFFFE, *header.size);
}

TEST(codecs_isobmff_Parser, parseBoxHeader_whenSizeExceedsLimit_thenThrows) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\xFF"
      "TEST",
      4 + 4));
  Parser parser(imageSource, 64);
  ASSERT_SPECTRUM_THROW(parser.parseBoxHeader(), error::IsoBmffBoxTooLarge);
}

TEST(
    codecs_isobmff_Parser,
    parseBoxHeader_whenExtraLargeSizeExceedsLimit_thenThrows) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x01"
      "TEST"
      "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFE",
      4 + 4 + 8));
  Parser parser(imageSource, 64);
  ASSERT_SPECTRUM_THROW(parser.parseBoxHeader(), error::IsoBmffBoxTooLarge);
}

TEST(codecs_isobmff_Parser, parseBoxHeader_whenBoxTypeUuid_thenReadUserType) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x04"
      "uuid"
      "ABCDEFGHHGFEDCBA",
      4 + 4 + 16));
  Parser parser(imageSource);
  const auto header = parser.parseBoxHeader();
  ASSERT_EQ(4, *header.size);
  spectrum::testutils::assertArrayContent("uuid", header.boxType);
  spectrum::testutils::assertArrayContent("ABCDEFGHHGFEDCBA", header.userType);
}

//
// parseFtypBox
//

TEST(
    codecs_isobmff_Parser,
    parseFtypBox_whenTooShort_thenThrowsFtypBoxTooSmall) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x04"
      "TEST"
      "ABCD",
      4 + 4 + 4));
  Parser parser(imageSource);
  ASSERT_SPECTRUM_THROW(parser.parseFtypBox(), error::IsoBmffFtypBoxTooSmall);
}

TEST(codecs_isobmff_Parser, parseFtypBox_whenNotFtypBox_thenThrows) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x10"
      "TEST",
      4 + 4));
  Parser parser(imageSource);
  ASSERT_SPECTRUM_THROW(
      parser.parseFtypBox(), error::IsoBmffFtypBoxExpectedButNotFound);
}

TEST(
    codecs_isobmff_Parser,
    parseFtypBox_whenFtypButNoMajorBrand_thenThrowsEarlyStreamEnd) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x10"
      "ftyp",
      4 + 4));
  Parser parser(imageSource);
  ASSERT_SPECTRUM_THROW(parser.parseFtypBox(), error::IsoBmffEarlyStreamEnd);
}

TEST(
    codecs_isobmff_Parser,
    parseFtypBox_whenFtypButNoMinorBrand_thenThrowsEarlyStreamEnd) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x10"
      "ftyp"
      "MAJO",
      4 + 4 + 4));
  Parser parser(imageSource);
  ASSERT_SPECTRUM_THROW(parser.parseFtypBox(), error::IsoBmffEarlyStreamEnd);
}

TEST(
    codecs_isobmff_Parser,
    parseFtypBox_whenFtypHasSizeButNoCompBrands_thenThrowsEarlyStreamEnd) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x10"
      "ftyp"
      "MAJO"
      "MINO",
      4 + 4 + 4 + 4));
  Parser parser(imageSource);
  ASSERT_SPECTRUM_THROW(parser.parseFtypBox(), error::IsoBmffEarlyStreamEnd);
}

TEST(
    codecs_isobmff_Parser,
    parseFtypBox_whenFtypHasSizeButRemainingNotMultipleOfFour_thenThrows) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x11"
      "ftyp"
      "MAJO"
      "MINO",
      4 + 4 + 4 + 4));
  Parser parser(imageSource);
  ASSERT_ANY_THROW(parser.parseFtypBox());
}

TEST(
    codecs_isobmff_Parser,
    parseFtypBox_whenWellFormed_thenExtractsEverything) {
  auto imageSource = io::testutils::makeVectorImageSource(std::string(
      "\x00\x00\x00\x10"
      "ftyp"
      "MAJO"
      "MINO"
      "COM1"
      "COM2",
      4 + 4 + 4 + 4 + 8));
  Parser parser(imageSource);
  const auto ftypBox = parser.parseFtypBox();
  spectrum::testutils::assertArrayContent("ftyp", ftypBox.boxHeader.boxType);
  spectrum::testutils::assertArrayContent("MAJO", ftypBox.majorBrand);
  spectrum::testutils::assertArrayContent("MINO", ftypBox.minorVersion);
  spectrum::testutils::assertArrayContent("COM1", ftypBox.compatibleBrands[0]);
  spectrum::testutils::assertArrayContent("COM2", ftypBox.compatibleBrands[1]);
}

} // namespace test
} // namespace isobmff
} // namespace codecs
} // namespace spectrum
} // namespace facebook
