// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/io/RewindableImageSource.h>
#include <spectrum/testutils/TestUtils.h>

#include <algorithm>
#include <memory>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace io {
namespace test {

static constexpr auto ALPHABET = "abcdefghijklmnopqrstuvwxzy";

//
// Tests that are observable as normal reads
//

TEST(RewindableImageSource, whenJustReadFromSource_thenReadAsNormal) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};

  testutils::assertRead("abcdefghijklmnopqrst", 20, imageSource);
  testutils::assertRead("", 0, imageSource);
  testutils::assertRead("uvwxzy", 20, imageSource);
  testutils::assertRead("", 20, imageSource);
}

TEST(RewindableImageSource, whenJustMarked_thenReadAsNormal) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};
  imageSource.mark();

  testutils::assertRead("abcdefghijklmnopqrst", 20, imageSource);
  testutils::assertRead("uvwxzy", 20, imageSource);
  testutils::assertRead("", 20, imageSource);
}

TEST(RewindableImageSource, whenJustReset_thenThrow) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};
  ASSERT_THROW(imageSource.reset(), SpectrumException);
}

TEST(RewindableImageSource, whenMarkedAndThenReset_thenReadAsNormal) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};
  imageSource.mark();
  imageSource.reset();

  testutils::assertRead("abcdefghijklmnopqrst", 20, imageSource);
  testutils::assertRead("uvwxzy", 20, imageSource);
  testutils::assertRead("", 20, imageSource);
}

TEST(
    RewindableImageSource,
    whenMarkedAndThenResetAndThenMark_thenReadAsNormal) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};
  imageSource.mark();
  imageSource.reset();
  imageSource.mark();

  testutils::assertRead("abcdefghijklmnopqrst", 20, imageSource);
  testutils::assertRead("uvwxzy", 20, imageSource);
  testutils::assertRead("", 20, imageSource);
}

TEST(RewindableImageSource, whenRewindableWithinRewindable_thenReadAsNormal) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};
  auto outerSource =
      RewindableImageSource{static_cast<IImageSource&>(imageSource)};

  testutils::assertRead("abcdefghijklmnopqrst", 20, outerSource);
  testutils::assertRead("uvwxzy", 20, outerSource);
  testutils::assertRead("", 20, outerSource);
}

//
// Tests with observable replay behaviour
//

TEST(
    RewindableImageSource,
    whenSequenceMarkReadResetRead_thenFirstPartReadTwice) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};

  imageSource.mark();

  testutils::assertRead("abcde", 5, imageSource);
  testutils::assertRead("fghij", 5, imageSource);

  imageSource.reset();

  testutils::assertRead("abcdefghijklmnopqrst", 20, imageSource);
  testutils::assertRead("uvwxzy", 20, imageSource);
  testutils::assertRead("", 20, imageSource);
}

TEST(
    RewindableImageSource,
    whenSequenceMarkReadResetMarkReadResetRead_thenFirstPartReadThreeTimes) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};

  imageSource.mark();

  testutils::assertRead("abcde", 5, imageSource);
  testutils::assertRead("fghij", 5, imageSource);

  imageSource.reset();
  imageSource.mark();

  testutils::assertRead("abcde", 5, imageSource);
  testutils::assertRead("fghij", 5, imageSource);

  imageSource.reset();

  testutils::assertRead("abcdefghijklmnopqrst", 20, imageSource);
  testutils::assertRead("uvwxzy", 20, imageSource);
  testutils::assertRead("", 20, imageSource);
}

TEST(
    RewindableImageSource,
    whenSequenceMarkReadResetReadMarkRead_thenFirstPartReadTwice) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};

  imageSource.mark();

  testutils::assertRead("abcde", 5, imageSource);
  testutils::assertRead("fghij", 5, imageSource);

  imageSource.reset();

  testutils::assertRead("abcdefghijklmnopqrst", 20, imageSource);

  imageSource.mark();

  testutils::assertRead("uvwxzy", 20, imageSource);
  testutils::assertRead("", 20, imageSource);
}

TEST(
    RewindableImageSource,
    whenSequenceMarkReadResetReadMarkResetRead_thenFirstPartTwiceAndSubpartThreeTimes) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};

  imageSource.mark();

  testutils::assertRead("abcde", 5, imageSource);

  imageSource.reset();
  imageSource.mark();

  testutils::assertRead("abc", 3, imageSource);

  imageSource.mark();

  testutils::assertRead("def", 3, imageSource);

  imageSource.reset();

  testutils::assertRead("defghijklmnopqrst", 17, imageSource);
  testutils::assertRead("uvwxzy", 20, imageSource);
  testutils::assertRead("", 20, imageSource);
}

//
// Test regarding total bytes read counter
//

TEST(
    RewindableImageSource,
    whenSequenceReadAndReread_thenByteCounterReflectsUnderlyingStream) {
  auto fakeSource = io::testutils::makeVectorImageSource(ALPHABET);
  auto imageSource = RewindableImageSource{fakeSource};

  imageSource.mark();

  testutils::assertRead("abcde", 5, imageSource);
  ASSERT_EQ(5, imageSource.getTotalBytesRead());

  imageSource.reset();
  imageSource.mark();

  testutils::assertRead("abc", 3, imageSource);
  ASSERT_EQ(5, imageSource.getTotalBytesRead());

  imageSource.mark();

  testutils::assertRead("def", 3, imageSource);
  ASSERT_EQ(6, imageSource.getTotalBytesRead());

  imageSource.reset();

  testutils::assertRead("defghijklmnopqrst", 17, imageSource);
  ASSERT_EQ(20, imageSource.getTotalBytesRead());

  testutils::assertRead("uvwxzy", 20, imageSource);
  ASSERT_EQ(26, imageSource.getTotalBytesRead());

  testutils::assertRead("", 20, imageSource);
  ASSERT_EQ(26, imageSource.getTotalBytesRead());
}

//
// Tests for hasNextByte
//

TEST(RewindableImageSource, whenEmptySource_thenAvailable0) {
  auto fakeSource = io::testutils::makeVectorImageSource("");
  auto imageSource = RewindableImageSource{fakeSource};
  ASSERT_EQ(0, imageSource.available());
}

TEST(RewindableImageSource, whenSourceCompletelyRead_thenAvailable0) {
  auto fakeSource = io::testutils::makeVectorImageSource("abc");
  auto imageSource = RewindableImageSource{fakeSource};

  testutils::assertRead("abc", 3, imageSource);
  ASSERT_EQ(0, imageSource.available());
}

TEST(RewindableImageSource, whenSourceHasByteLeft_thenAvailableFullMinusRead) {
  auto fakeSource = io::testutils::makeVectorImageSource("abc");
  auto imageSource = RewindableImageSource{fakeSource};

  testutils::assertRead("ab", 2, imageSource);
  ASSERT_EQ(1, imageSource.available());
}

TEST(
    RewindableImageSource,
    whenSourceCompletelyReadButThenResetted_thenAvailableFull) {
  auto fakeSource = io::testutils::makeVectorImageSource("abc");
  auto imageSource = RewindableImageSource{fakeSource};

  imageSource.mark();
  testutils::assertRead("abc", 3, imageSource);
  imageSource.reset();
  ASSERT_EQ(3, imageSource.available());
}

} // namespace test
} // namespace io
} // namespace spectrum
} // namespace facebook
