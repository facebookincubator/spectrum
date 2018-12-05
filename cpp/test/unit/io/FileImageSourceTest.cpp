// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/io/FileImageSource.h>
#include <spectrum/testutils/TestUtils.h>

#include <array>
#include <cstddef>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace io {
namespace test {

TEST(FileImageSource, whenOpeningNonExistingFile_thenThrow) {
  ASSERT_THROW(
      FileImageSource{testdata::paths::NON_EXISTING_FILE.normalized()},
      std::runtime_error);
}

TEST(FileImageSource, whenOpeningExistingFile_thenNotThrown) {
  FileImageSource source(
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized());
}

TEST(FileImageSource, whenReadingExistingFile_thenNotEmptyResultReturned) {
  FileImageSource source(
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized());

  std::array<char, 256> buf;
  buf.fill('\0');

  const std::size_t bytesRead = source.read(buf.data(), buf.size());
  ASSERT_GT(bytesRead, 0);

  bool arrayHasBeenChanged = false;
  for (const auto c : buf) {
    if (c != '\0') {
      arrayHasBeenChanged = true;
      break;
    }
  }
  ASSERT_TRUE(arrayHasBeenChanged);
}

TEST(FileImageSource, whenReadingFile_thenReadBytesMatchBytesInFile) {
  FileImageSource source(testdata::paths::misc::alphabetTxt.normalized());

  testutils::assertRead("a", 1, source);
  testutils::assertRead("bcdefghijklmnopqrstuvwxyz", 30, source);
  testutils::assertRead("", 30, source);
}

TEST(
    FileImageSource,
    whenReadingFile_thenTotalBytesReadMatchBytesActuallyRead) {
  FileImageSource source(
      testdata::paths::jpeg::s128x85_Q75_BASELINE.normalized());

  std::size_t totalBytesRead = 0;
  std::array<char, 256> buf;

  std::size_t bytesRead;
  while ((bytesRead = source.read(buf.data(), buf.size()))) {
    totalBytesRead += bytesRead;
  };

  ASSERT_EQ(totalBytesRead, source.getTotalBytesRead());
}

TEST(FileImageSource, whenAvailableOnBegin_thenMatchesFileSize) {
  FileImageSource source(testdata::paths::misc::alphabetTxt.normalized());

  ASSERT_EQ(26, source.available());
}

TEST(FileImageSource, whenAvailableAfterRead_thenMatchesRemainingBufferLength) {
  FileImageSource source(testdata::paths::misc::alphabetTxt.normalized());

  testutils::assertRead("abcdefghij", 10, source);
  ASSERT_EQ(26 - 10, source.available());
}

} // namespace test
} // namespace io
} // namespace spectrum
} // namespace facebook
