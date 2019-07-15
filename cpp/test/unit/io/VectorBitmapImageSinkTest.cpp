// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/io/VectorImageSink.h>

#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace io {
namespace test {

namespace {
std::string vectorToString(std::vector<char> v) {
  return std::string(v.begin(), v.end());
}
} // namespace

TEST(VectorBitmapImageSink, whenConstructed_thenContentEmpty) {
  VectorBitmapImageSink sink;
  ASSERT_EQ("", vectorToString(sink.getVectorReference()));
}

TEST(
    VectorBitmapImageSink,
    whenGivenConfiguration_thenNothingObservableHappens) {
  VectorBitmapImageSink sink;
  sink.setConfiguration(
      image::Size{640, 480}, image::pixel::specifications::ARGB);
  ASSERT_EQ("", vectorToString(sink.getVectorReference()));
}

TEST(VectorBitmapImageSink, whenWrittenTo_thenAppendedToContent) {
  VectorBitmapImageSink sink;
  ASSERT_EQ("", vectorToString(sink.getVectorReference()));

  std::string s1 = "abcdefg";
  sink.write(s1.data(), s1.size());
  ASSERT_EQ("abcdefg", vectorToString(sink.getVectorReference()));

  std::string s2 = "123456789";
  sink.write(s2.data(), s2.size());
  ASSERT_EQ("abcdefg123456789", vectorToString(sink.getVectorReference()));
}

TEST(
    VectorBitmapImageSink,
    whenWrittenpartiallyTo_thenPartiallyAppendedToContent) {
  VectorBitmapImageSink sink;
  ASSERT_EQ("", vectorToString(sink.getVectorReference()));

  std::string s1 = "abcdefg";
  sink.write(s1.data(), 3);
  ASSERT_EQ("abc", vectorToString(sink.getVectorReference()));

  std::string s2 = "123456789";
  sink.write(s2.data(), 3);
  ASSERT_EQ("abc123", vectorToString(sink.getVectorReference()));
}

TEST(VectorBitmapImageSink, whenWrittenTo_thenBytesWrittenCounterIncreases) {
  VectorBitmapImageSink sink;
  ASSERT_EQ("", vectorToString(sink.getVectorReference()));

  std::string s1 = "abcdefg";
  sink.write(s1.data(), s1.size());
  ASSERT_EQ(7, sink.totalBytesWritten());

  std::string s2 = "123456789";
  sink.write(s2.data(), s2.size());
  ASSERT_EQ(16, sink.totalBytesWritten());
}

} // namespace test
} // namespace io
} // namespace spectrum
} // namespace facebook
