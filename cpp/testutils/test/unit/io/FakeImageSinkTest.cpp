// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/testutils/TestUtils.h>

#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace io {
namespace testutils {
namespace test {

TEST(io_testutils_FakeImageSink, whenConstructed_thenContentEmpty) {
  auto sink = FakeImageSink{};
  ASSERT_EQ("", sink.stringContent());
  ASSERT_FALSE(sink.size().hasValue());
  ASSERT_FALSE(sink.getPixelSpecification().hasValue());
}

TEST(io_testutils_FakeImageSink, whenGivenConfiguration_thenParametersSet) {
  auto sink = FakeImageSink{};
  sink.setConfiguration(
      image::Size{640, 480}, image::pixel::specifications::ARGB);
  ASSERT_EQ(640, sink.size()->width);
  ASSERT_EQ(480, sink.size()->height);
  ASSERT_EQ(image::pixel::specifications::ARGB, sink.getPixelSpecification());
}

TEST(io_testutils_FakeImageSink, whenWrittenTo_thenAppendedToContent) {
  auto sink = FakeImageSink{};
  ASSERT_EQ("", sink.stringContent());

  std::string s1 = "abcdefg";
  sink.write(s1.data(), s1.size());
  ASSERT_EQ("abcdefg", sink.stringContent());

  std::string s2 = "123456789";
  sink.write(s2.data(), s2.size());
  ASSERT_EQ("abcdefg123456789", sink.stringContent());
}

TEST(
    io_testutils_FakeImageSink,
    whenWrittenpartiallyTo_thenPartiallyAppendedToContent) {
  auto sink = FakeImageSink{};
  ASSERT_EQ("", sink.stringContent());

  std::string s1 = "abcdefg";
  sink.write(s1.data(), 3);
  ASSERT_EQ("abc", sink.stringContent());

  std::string s2 = "123456789";
  sink.write(s2.data(), 3);
  ASSERT_EQ("abc123", sink.stringContent());
}

TEST(
    io_testutils_FakeImageSink,
    whenWrittenTo_thenBytesWrittenCounterIncreases) {
  auto sink = FakeImageSink{};
  ASSERT_EQ("", sink.stringContent());

  std::string s1 = "abcdefg";
  sink.write(s1.data(), s1.size());
  ASSERT_EQ(7, sink.totalBytesWritten());

  std::string s2 = "123456789";
  sink.write(s2.data(), s2.size());
  ASSERT_EQ(16, sink.totalBytesWritten());
}

} // namespace helper
} // namespace testutils
} // namespace io
} // namespace spectrum
} // namespace facebook
