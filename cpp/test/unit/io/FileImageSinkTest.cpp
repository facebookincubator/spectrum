// Copyright (c) Facebook, Inc. and its affiliates.
//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.

#include <spectrum/io/FileImageSink.h>

#include <array>
#include <cstddef>
#include <cstdio>
#include <string>

#include <gtest/gtest.h>

namespace facebook {
namespace spectrum {
namespace io {
namespace test {

TEST(FileImageSink, whenOpeningDevNull_thenNothingThrows) {
  FileImageSink sink("/dev/null");
}

TEST(FileImageSink, whenSettingConfiguration_thenNothingHappens) {
  FileImageSink sink("/dev/null");
  sink.setConfiguration(
      image::Size{480, 360}, image::pixel::specifications::RGBA);
}

TEST(FileImageSink, whenOpeningFileInNonExistingDirectory_thenThrows) {
  ASSERT_THROW(
      FileImageSink sink("/this/path/does/not/exist/spectrum"),
      SpectrumException);
}

TEST(FileImageSink, whenWritingToDevNull_thenWriteCountIncreases) {
  FileImageSink sink("/dev/null");

  ASSERT_EQ(0, sink.totalBytesWritten());
  sink.write("abcde", 5);
  ASSERT_EQ(5, sink.totalBytesWritten());
}

} // namespace test
} // namespace io
} // namespace spectrum
} // namespace facebook
